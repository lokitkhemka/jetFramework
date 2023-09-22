#include<Geometry/Sphere/sphere3.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Sphere3, Constructors) {
    Sphere3 sph1;
    EXPECT_DOUBLE_EQ(0.0, sph1.Center.x);
    EXPECT_DOUBLE_EQ(0.0, sph1.Center.y);
    EXPECT_DOUBLE_EQ(0.0, sph1.Center.z);
    EXPECT_DOUBLE_EQ(1.0, sph1.Radius);

    Sphere3 sph2({3.0, -1.0, 2.0}, 5.0);
    EXPECT_DOUBLE_EQ(3.0, sph2.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph2.Center.y);
    EXPECT_DOUBLE_EQ(2.0, sph2.Center.z);
    EXPECT_DOUBLE_EQ(5.0, sph2.Radius);

    sph2.IsNormalFlipped = true;

    Sphere3 sph3(sph2);
    EXPECT_DOUBLE_EQ(3.0, sph3.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph3.Center.y);
    EXPECT_DOUBLE_EQ(2.0, sph3.Center.z);
    EXPECT_DOUBLE_EQ(5.0, sph3.Radius);
    EXPECT_TRUE(sph3.IsNormalFlipped);
}

TEST(Sphere3, ClosestPoint) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);

    auto result1 = sph.ClosestPoint({10.0, -1.0, 2.0});
    EXPECT_DOUBLE_EQ(8.0, result1.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.y);
    EXPECT_DOUBLE_EQ(2.0, result1.z);

    auto result2 = sph.ClosestPoint({3.0, -10.0, 2.0});
    EXPECT_DOUBLE_EQ(3.0, result2.x);
    EXPECT_DOUBLE_EQ(-6.0, result2.y);
    EXPECT_DOUBLE_EQ(2.0, result2.z);

    auto result3 = sph.ClosestPoint({3.0, 3.0, 2.0});
    EXPECT_DOUBLE_EQ(3.0, result3.x);
    EXPECT_DOUBLE_EQ(4.0, result3.y);
    EXPECT_DOUBLE_EQ(2.0, result3.z);
}

TEST(Sphere3, ClosestDistance) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);

    double result1 = sph.ClosestDistance({10.0, -1.0, 2.0});
    EXPECT_DOUBLE_EQ(2.0, result1);

    double result2 = sph.ClosestDistance({3.0, -10.0, 2.0});
    EXPECT_DOUBLE_EQ(4.0, result2);

    double result3 = sph.ClosestDistance({3.0, 3.0, 2.0});
    EXPECT_DOUBLE_EQ(1.0, result3);
}

TEST(Sphere3, Intersects) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);
    sph.IsNormalFlipped = true;

    bool result1 = sph.Intersects(Ray3D({10.0, -1.0, 2.0}, {-1.0, 0.0, 0.0}));
    EXPECT_TRUE(result1);

    bool result2 = sph.Intersects(Ray3D({3.0, -10.0, 2.0}, {0.0, -1.0, 0.0}));
    EXPECT_FALSE(result2);

    bool result3 = sph.Intersects(Ray3D({3.0, 3.0, 2.0}, {1.0, 0.0, 0.0}));
    EXPECT_TRUE(result3);
}

TEST(Sphere3, ClosestIntersection) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);
    sph.IsNormalFlipped = true;

    auto result1 = sph.ClosestIntersection(
        Ray3D({10.0, -1.0, 2.0}, {-1.0, 0.0, 0.0}));
    EXPECT_TRUE(result1.IsIntersecting);
    EXPECT_DOUBLE_EQ(2.0, result1.t);
    EXPECT_DOUBLE_EQ(8.0, result1.Point.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.Point.y);
    EXPECT_DOUBLE_EQ(2.0, result1.Point.z);
    EXPECT_DOUBLE_EQ(-1.0, result1.Normal.x);
    EXPECT_DOUBLE_EQ(0.0, result1.Normal.y);
    EXPECT_DOUBLE_EQ(0.0, result1.Normal.z);

    auto result2 = sph.ClosestIntersection(
        Ray3D({3.0, -10.0, 2.0}, {0.0, -1.0, 0.0}));
    EXPECT_FALSE(result2.IsIntersecting);

    auto result3 = sph.ClosestIntersection(
        Ray3D({3.0, 3.0, 2.0}, {0.0, 1.0, 0.0}));
    EXPECT_TRUE(result3.IsIntersecting);
    EXPECT_DOUBLE_EQ(1.0, result3.t);
    EXPECT_DOUBLE_EQ(3.0, result3.Point.x);
    EXPECT_DOUBLE_EQ(4.0, result3.Point.y);
    EXPECT_DOUBLE_EQ(2.0, result3.Point.z);
    EXPECT_DOUBLE_EQ(0.0, result3.Normal.x);
    EXPECT_DOUBLE_EQ(-1.0, result3.Normal.y);
    EXPECT_DOUBLE_EQ(0.0, result3.Normal.z);
}

TEST(Sphere3, BoundingBox) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);
    BoundingBox3D bbox = sph.BoundingBox();

    EXPECT_DOUBLE_EQ(-2.0, bbox.LowerCorner.x);
    EXPECT_DOUBLE_EQ(-6.0, bbox.LowerCorner.y);
    EXPECT_DOUBLE_EQ(-3.0, bbox.LowerCorner.z);
    EXPECT_DOUBLE_EQ(8.0, bbox.UpperCorner.x);
    EXPECT_DOUBLE_EQ(4.0, bbox.UpperCorner.y);
    EXPECT_DOUBLE_EQ(7.0, bbox.UpperCorner.z);
}

TEST(Sphere3, ClosestNormal) {
    Sphere3 sph({3.0, -1.0, 2.0}, 5.0);
    sph.IsNormalFlipped = true;

    auto result1 = sph.ClosestNormal({10.0, -1.0, 2.0});
    EXPECT_DOUBLE_EQ(-1.0, result1.x);
    EXPECT_DOUBLE_EQ(0.0, result1.y);
    EXPECT_DOUBLE_EQ(0.0, result1.z);

    auto result2 = sph.ClosestNormal({3.0, -10.0, 2.0});
    EXPECT_DOUBLE_EQ(0.0, result2.x);
    EXPECT_DOUBLE_EQ(1.0, result2.y);
    EXPECT_DOUBLE_EQ(0.0, result2.z);

    auto result3 = sph.ClosestNormal({3.0, 3.0, 2.0});
    EXPECT_DOUBLE_EQ(0.0, result3.x);
    EXPECT_DOUBLE_EQ(-1.0, result3.y);
    EXPECT_DOUBLE_EQ(0.0, result3.z);
}

TEST(Sphere3, Builder) {
    Sphere3 sph = Sphere3::builder()
        .WithCenter({3.0, -1.0, 2.0})
        .WithRadius(5.0)
        .WithIsNormalFlipped(true)
        .Build();
    EXPECT_DOUBLE_EQ(3.0, sph.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph.Center.y);
    EXPECT_DOUBLE_EQ(2.0, sph.Center.z);
    EXPECT_DOUBLE_EQ(5.0, sph.Radius);
    EXPECT_TRUE(sph.IsNormalFlipped);
}