#ifndef OPENMW_COMPONENTS_DETOURNAVIGATOR_TILECACHEDRECASTMESHMANAGER_H
#define OPENMW_COMPONENTS_DETOURNAVIGATOR_TILECACHEDRECASTMESHMANAGER_H

#include "cachedrecastmeshmanager.hpp"
#include "tileposition.hpp"
#include "settingsutils.hpp"
#include "gettilespositions.hpp"

#include <components/misc/guarded.hpp>

#include <map>
#include <mutex>
#include <set>

namespace DetourNavigator
{
    class TileCachedRecastMeshManager
    {
    public:
        TileCachedRecastMeshManager(const Settings& settings);

        bool addObject(const ObjectId id, const btCollisionShape& shape, const btTransform& transform,
                       const AreaType areaType);

        template <class OnChangedTile>
        bool updateObject(const ObjectId id, const btCollisionShape& shape, const btTransform& transform,
            const AreaType areaType, OnChangedTile&& onChangedTile)
        {
            const auto object = mObjectsTilesPositions.find(id);
            if (object == mObjectsTilesPositions.end())
                return false;
            auto& currentTiles = object->second;
            const auto border = getBorderSize(mSettings);
            bool changed = false;
            std::set<TilePosition> newTiles;
            {
                auto tiles = mTiles.lock();
                const auto onTilePosition = [&] (const TilePosition& tilePosition)
                {
                    if (currentTiles.count(tilePosition))
                    {
                        newTiles.insert(tilePosition);
                        if (updateTile(id, transform, areaType, tilePosition, tiles.get()))
                        {
                            onChangedTile(tilePosition);
                            changed = true;
                        }
                    }
                    else if (addTile(id, shape, transform, areaType, tilePosition, border, tiles.get()))
                    {
                        newTiles.insert(tilePosition);
                        onChangedTile(tilePosition);
                        changed = true;
                    }
                };
                getTilesPositions(shape, transform, mSettings, onTilePosition);
                for (const auto& tile : currentTiles)
                {
                    if (!newTiles.count(tile) && removeTile(id, tile, tiles.get()))
                    {
                        onChangedTile(tile);
                        changed = true;
                    }
                }
            }
            std::swap(currentTiles, newTiles);
            if (changed)
                ++mRevision;
            return changed;
        }

        std::optional<RemovedRecastMeshObject> removeObject(const ObjectId id);

        bool addWater(const osg::Vec2i& cellPosition, const int cellSize, const btTransform& transform);

        std::optional<RecastMeshManager::Water> removeWater(const osg::Vec2i& cellPosition);

        std::shared_ptr<RecastMesh> getMesh(const TilePosition& tilePosition);

        bool hasTile(const TilePosition& tilePosition);

        template <class Function>
        void forEachTilePosition(Function&& function)
        {
            for (const auto& tile : *mTiles.lock())
                function(tile.first);
        }

        std::size_t getRevision() const;

    private:
        const Settings& mSettings;
        Misc::ScopeGuarded<std::map<TilePosition, CachedRecastMeshManager>> mTiles;
        std::unordered_map<ObjectId, std::set<TilePosition>> mObjectsTilesPositions;
        std::map<osg::Vec2i, std::vector<TilePosition>> mWaterTilesPositions;
        std::size_t mRevision = 0;
        std::size_t mTilesGeneration = 0;

        bool addTile(const ObjectId id, const btCollisionShape& shape, const btTransform& transform,
                     const AreaType areaType, const TilePosition& tilePosition, float border,
                     std::map<TilePosition, CachedRecastMeshManager>& tiles);

        bool updateTile(const ObjectId id, const btTransform& transform, const AreaType areaType,
                        const TilePosition& tilePosition, std::map<TilePosition, CachedRecastMeshManager>& tiles);

        std::optional<RemovedRecastMeshObject> removeTile(const ObjectId id, const TilePosition& tilePosition,
                                                            std::map<TilePosition, CachedRecastMeshManager>& tiles);
    };
}

#endif
