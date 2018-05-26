#include "asyncnavmeshupdater.hpp"
#include "debug.hpp"
#include "makenavmesh.hpp"
#include "settings.hpp"

#include <components/debug/debuglog.hpp>

#include <iostream>

namespace
{
    using DetourNavigator::TilePosition;

    int getDistance(const TilePosition& lhs, const TilePosition& rhs)
    {
        return std::abs(lhs.x() - rhs.x()) + std::abs(lhs.y() - rhs.y());
    }

    std::pair<int, int> makePriority(const TilePosition& changedTile, const TilePosition& playerTile)
    {
        return std::make_pair(getDistance(changedTile, playerTile), getDistance(changedTile, TilePosition {0, 0}));
    }
}

namespace DetourNavigator
{
    static std::ostream& operator <<(std::ostream& stream, UpdateNavMeshStatus value)
    {
        switch (value)
        {
            case UpdateNavMeshStatus::ignore:
                return stream << "ignore";
            case UpdateNavMeshStatus::removed:
                return stream << "removed";
            case UpdateNavMeshStatus::add:
                return stream << "add";
            case UpdateNavMeshStatus::replaced:
                return stream << "replaced";
        }
        return stream << "unknown";
    }

    AsyncNavMeshUpdater::AsyncNavMeshUpdater(const Settings& settings, TileCachedRecastMeshManager& recastMeshManager)
        : mSettings(settings)
        , mRecastMeshManager(recastMeshManager)
        , mShouldStop()
        , mThread([&] { process(); })
    {
    }

    AsyncNavMeshUpdater::~AsyncNavMeshUpdater()
    {
        mShouldStop = true;
        std::unique_lock<std::mutex> lock(mMutex);
        mJobs = decltype(mJobs)();
        mHasJob.notify_all();
        lock.unlock();
        mThread.join();
    }

    void AsyncNavMeshUpdater::post(const osg::Vec3f& agentHalfExtents,
        const std::shared_ptr<NavMeshCacheItem>& navMeshCacheItem, const TilePosition& playerTile,
        const std::set<TilePosition>& changedTiles)
    {
        log("post jobs playerTile=", playerTile);

        setPlayerTile(playerTile);

        if (changedTiles.empty())
            return;

        const std::lock_guard<std::mutex> lock(mMutex);

        for (const auto& changedTile : changedTiles)
        {
            if (mPushed[agentHalfExtents].insert(changedTile).second)
                mJobs.push(Job {agentHalfExtents, navMeshCacheItem, changedTile,
                                makePriority(changedTile, playerTile)});
        }

        log("posted ", mJobs.size(), " jobs");

        mHasJob.notify_all();
    }

    void AsyncNavMeshUpdater::wait()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mDone.wait(lock, [&] { return mJobs.empty(); });
    }

    void AsyncNavMeshUpdater::process() throw()
    {
        log("start process jobs");
        while (!mShouldStop)
        {
            try
            {
                if (const auto job = getNextJob())
                    processJob(*job);
            }
            catch (const std::exception& e)
            {
                DetourNavigator::log("AsyncNavMeshUpdater::process exception: ", e.what());
                ::Log(Debug::Error) << "Exception while process navmesh updated job: " << e.what();
            }
        }
        log("stop process jobs");
    }

    void AsyncNavMeshUpdater::processJob(const Job& job)
    {
        log("process job for agent=", job.mAgentHalfExtents);

        const auto start = std::chrono::steady_clock::now();

        setFirstStart(start);

        const auto recastMesh = mRecastMeshManager.get().getMesh(job.mChangedTile);
        const auto playerTile = getPlayerTile();

        const auto status = updateNavMesh(job.mAgentHalfExtents, recastMesh.get(), job.mChangedTile, playerTile,
            mSettings, *job.mNavMeshCacheItem);

        const auto finish = std::chrono::steady_clock::now();

        writeDebugFiles(job, recastMesh.get());

        using FloatMs = std::chrono::duration<float, std::milli>;

        log("cache updated for agent=", job.mAgentHalfExtents, " status=", status,
            " generation=", job.mNavMeshCacheItem->mGeneration,
            " revision=", job.mNavMeshCacheItem->mNavMeshRevision,
            " time=", std::chrono::duration_cast<FloatMs>(finish - start).count(), "ms",
            " total_time=", std::chrono::duration_cast<FloatMs>(finish - getFirstStart()).count(), "ms");
    }

    boost::optional<AsyncNavMeshUpdater::Job> AsyncNavMeshUpdater::getNextJob()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mJobs.empty())
            mHasJob.wait_for(lock, std::chrono::milliseconds(10));
        if (mJobs.empty())
        {
            mDone.notify_all();
            return boost::none;
        }
        log("got ", mJobs.size(), " jobs");
        const auto job = mJobs.top();
        mJobs.pop();
        const auto pushed = mPushed.find(job.mAgentHalfExtents);
        pushed->second.erase(job.mChangedTile);
        if (pushed->second.empty())
            mPushed.erase(pushed);
        return job;
    }

    void AsyncNavMeshUpdater::writeDebugFiles(const Job& job, const RecastMesh* recastMesh) const
    {
        std::string revision;
        std::string recastMeshRevision;
        std::string navMeshRevision;
        if ((mSettings.get().mEnableWriteNavMeshToFile || mSettings.get().mEnableWriteRecastMeshToFile)
                && (mSettings.get().mEnableRecastMeshFileNameRevision || mSettings.get().mEnableNavMeshFileNameRevision))
        {
            revision = "." + std::to_string((std::chrono::steady_clock::now()
                - std::chrono::steady_clock::time_point()).count());
            if (mSettings.get().mEnableRecastMeshFileNameRevision)
                recastMeshRevision = revision;
            if (mSettings.get().mEnableNavMeshFileNameRevision)
                navMeshRevision = revision;
        }
        if (recastMesh && mSettings.get().mEnableWriteRecastMeshToFile)
            writeToFile(*recastMesh, mSettings.get().mRecastMeshPathPrefix + std::to_string(job.mChangedTile.x())
                        + "_" + std::to_string(job.mChangedTile.y()) + "_", recastMeshRevision);
        if (mSettings.get().mEnableWriteNavMeshToFile)
            writeToFile(*job.mNavMeshCacheItem->mValue.lock(), mSettings.get().mNavMeshPathPrefix, navMeshRevision);
    }

    std::chrono::steady_clock::time_point AsyncNavMeshUpdater::getFirstStart()
    {
        const std::lock_guard<std::mutex> lock(mFirstStartMutex);
        return *mFirstStart;
    }

    void AsyncNavMeshUpdater::setFirstStart(const std::chrono::steady_clock::time_point& value)
    {
        const std::lock_guard<std::mutex> lock(mFirstStartMutex);
        if (!mFirstStart)
            mFirstStart = value;
    }

    TilePosition AsyncNavMeshUpdater::getPlayerTile()
    {
        const std::lock_guard<std::mutex> lock(mPlayerTileMutex);
        return mPlayerTile;
    }

    void AsyncNavMeshUpdater::setPlayerTile(const TilePosition& value)
    {
        const std::lock_guard<std::mutex> lock(mPlayerTileMutex);
        mPlayerTile = value;
    }
}
