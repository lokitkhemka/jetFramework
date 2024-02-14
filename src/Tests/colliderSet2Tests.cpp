#include <Geometry/Box/box2.h>
#include <ParticleSim/Collision/collider2_set.h>
#include <ParticleSim/Collision/rigid_body2_collider.h>
#include <vector>
#include <gtest/gtest.h>

using namespace jet;

TEST(ColliderSet2, Constructors) {
    auto box1 = Box2::builder()
        .WithLowerCorner({0, 1})
        .WithUpperCorner({1, 2})
        .MakeShared();

    auto box2 = Box2::builder()
        .WithLowerCorner({2, 3})
        .WithUpperCorner({3, 4})
        .MakeShared();

    auto col1 = RigidBodyCollider2::builder()
        .WithSurface(box1)
        .MakeShared();

    auto col2 = RigidBodyCollider2::builder()
        .WithSurface(box2)
        .MakeShared();

    ColliderSet2 colSet1;
    EXPECT_EQ(0u, colSet1.NumberOfColliders());

    ColliderSet2 colSet2({col1, col2});
    EXPECT_EQ(2u, colSet2.NumberOfColliders());
    EXPECT_EQ(col1, colSet2.Collider(0));
    EXPECT_EQ(col2, colSet2.Collider(1));
}

TEST(ColliderSet2, Builder) {
    auto box1 = Box2::builder()
        .WithLowerCorner({0, 1})
        .WithUpperCorner({1, 2})
        .MakeShared();

    auto box2 = Box2::builder()
        .WithLowerCorner({2, 3})
        .WithUpperCorner({3, 4})
        .MakeShared();

    auto col1 = RigidBodyCollider2::builder()
        .WithSurface(box1)
        .MakeShared();

    auto col2 = RigidBodyCollider2::builder()
        .WithSurface(box2)
        .MakeShared();

    auto colSet1 = ColliderSet2::builder().MakeShared();
    EXPECT_EQ(0u, colSet1->NumberOfColliders());

    auto colSet2 = ColliderSet2::builder()
        .WithColliders({col1, col2})
        .MakeShared();
    EXPECT_EQ(2u, colSet2->NumberOfColliders());
    EXPECT_EQ(col1, colSet2->Collider(0));
    EXPECT_EQ(col2, colSet2->Collider(1));

    auto colSet3 = ColliderSet2::builder().Build();
    EXPECT_EQ(0u, colSet3.NumberOfColliders());
}
