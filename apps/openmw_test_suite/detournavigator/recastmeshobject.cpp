#include "operators.hpp"

#include <components/detournavigator/recastmeshobject.hpp>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>

#include <gtest/gtest.h>

namespace
{
    using namespace testing;
    using namespace DetourNavigator;

    struct DetourNavigatorRecastMeshObjectTest : Test
    {
        btBoxShape mBoxShape {btVector3(1, 2, 3)};
        btCompoundShape mCompoundShape {btVector3(1, 2, 3)};
        btTransform mTransform {btQuaternion(btVector3(1, 2, 3), 1), btVector3(1, 2, 3)};

        DetourNavigatorRecastMeshObjectTest()
        {
            mCompoundShape.addChildShape(mTransform, std::addressof(mBoxShape));
        }
    };

    TEST_F(DetourNavigatorRecastMeshObjectTest, constructed_object_should_have_shape_and_transform)
    {
        const RecastMeshObject object(mBoxShape, mTransform);
        EXPECT_EQ(std::addressof(object.getShape()), std::addressof(mBoxShape));
        EXPECT_EQ(object.getTransform(), mTransform);
    }

    TEST_F(DetourNavigatorRecastMeshObjectTest, update_with_same_transform_for_not_compound_shape_should_return_false)
    {
        RecastMeshObject object(mBoxShape, mTransform);
        EXPECT_FALSE(object.update(mTransform));
    }

    TEST_F(DetourNavigatorRecastMeshObjectTest, update_with_different_transform_should_return_true)
    {
        RecastMeshObject object(mBoxShape, mTransform);
        EXPECT_TRUE(object.update(btTransform::getIdentity()));
    }

    TEST_F(DetourNavigatorRecastMeshObjectTest, update_for_compound_shape_with_same_transform_and_not_changed_child_transform_should_return_false)
    {
        RecastMeshObject object(mCompoundShape, mTransform);
        EXPECT_FALSE(object.update(mTransform));
    }

    TEST_F(DetourNavigatorRecastMeshObjectTest, update_for_compound_shape_with_same_transform_and_changed_child_transform_should_return_true)
    {
        RecastMeshObject object(mCompoundShape, mTransform);
        mCompoundShape.updateChildTransform(0, btTransform::getIdentity());
        EXPECT_TRUE(object.update(mTransform));
    }

    TEST_F(DetourNavigatorRecastMeshObjectTest, repeated_update_for_compound_shape_without_changes_should_return_false)
    {
        RecastMeshObject object(mCompoundShape, mTransform);
        mCompoundShape.updateChildTransform(0, btTransform::getIdentity());
        object.update(mTransform);
        EXPECT_FALSE(object.update(mTransform));
    }
}
