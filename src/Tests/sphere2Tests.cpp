#include<Geometry/Sphere/sphere2.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Sphere2, Constructors)
{
    Sphere2 sp1;
    EXPECT_DOUBLE_EQ(0.0, sp1.Center.x);
    EXPECT_DOUBLE_EQ(0.0, sp1.Center.y);
    EXPECT_DOUBLE_EQ(1.0, sp1.Radius);

    Sphere2 sph2({3.0, -1.0}, 5.0);
    EXPECT_DOUBLE_EQ(3.0, sph2.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph2.Center.y);
    EXPECT_DOUBLE_EQ(5.0, sph2.Radius);

    sph2.IsNormalFlipped = true;

    Sphere2 sph3(sph2);
    EXPECT_DOUBLE_EQ(3.0, sph3.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph3.Center.y);
    EXPECT_DOUBLE_EQ(5.0, sph3.Radius);
    EXPECT_TRUE(sph3.IsNormalFlipped);
}

TEST(Sphere2, ClosestPoint) {
    Sphere2 sph({3.0, -1.0}, 5.0);

    auto result1 = sph.ClosestPoint({10.0, -1.0});
    EXPECT_DOUBLE_EQ(8.0, result1.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.y);

    auto result2 = sph.ClosestPoint({3.0, -10.0});
    EXPECT_DOUBLE_EQ(3.0, result2.x);
    EXPECT_DOUBLE_EQ(-6.0, result2.y);

    auto result3 = sph.ClosestPoint({3.0, 3.0});
    EXPECT_DOUBLE_EQ(3.0, result3.x);
    EXPECT_DOUBLE_EQ(4.0, result3.y);
}

TEST(Sphere2, ClosestDistance) {
    Sphere2 sph({3.0, -1.0}, 5.0);

    double result1 = sph.ClosestDistance({10.0, -1.0});
    EXPECT_DOUBLE_EQ(2.0, result1);

    double result2 = sph.ClosestDistance({3.0, -10.0});
    EXPECT_DOUBLE_EQ(4.0, result2);

    double result3 = sph.ClosestDistance({3.0, 3.0});
    EXPECT_DOUBLE_EQ(1.0, result3);
}


TEST(Sphere2, Intersects) {
    Sphere2 sph({3.0, -1.0}, 5.0);
    sph.IsNormalFlipped = true;

    bool result1 = sph.Intersects(Ray2D({10.0, -1.0}, {-1.0, 0.0}));
    EXPECT_TRUE(result1);

    bool result2 = sph.Intersects(Ray2D({3.0, -10.0}, {0.0, -1.0}));
    EXPECT_FALSE(result2);

    bool result3 = sph.Intersects(Ray2D({3.0, 3.0}, {1.0, 0.0}));
    EXPECT_TRUE(result3);
}

TEST(Sphere2, ClosestIntersection) {
    Sphere2 sph({3.0, -1.0}, 5.0);
    sph.IsNormalFlipped = true;

    auto result1 = sph.ClosestIntersection(Ray2D({10.0, -1.0}, {-1.0, 0.0}));
    EXPECT_TRUE(result1.IsIntersecting);
    EXPECT_DOUBLE_EQ(2.0, result1.t);
    EXPECT_DOUBLE_EQ(8.0, result1.Point.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.Point.y);

    auto result2 = sph.ClosestIntersection(Ray2D({3.0, -10.0}, {0.0, -1.0}));
    EXPECT_FALSE(result2.IsIntersecting);

    auto result3 = sph.ClosestIntersection(Ray2D({3.0, 3.0}, {0.0, 1.0}));
    EXPECT_TRUE(result3.IsIntersecting);
    EXPECT_DOUBLE_EQ(1.0, result3.t);
    EXPECT_DOUBLE_EQ(3.0, result3.Point.x);
    EXPECT_DOUBLE_EQ(4.0, result3.Point.y);
}

TEST(Sphere2, BoundingBox) {
    Sphere2 sph({3.0, -1.0}, 5.0);
    BoundingBox2D bbox = sph.BoundingBox();

    EXPECT_DOUBLE_EQ(-2.0, bbox.LowerCorner.x);
    EXPECT_DOUBLE_EQ(-6.0, bbox.LowerCorner.y);
    EXPECT_DOUBLE_EQ(8.0, bbox.UpperCorner.x);
    EXPECT_DOUBLE_EQ(4.0, bbox.UpperCorner.y);
}

TEST(Sphere2, ClosestNormal) {
    Sphere2 sph({3.0, -1.0}, 5.0);
    sph.IsNormalFlipped = true;

    auto result1 = sph.ClosestNormal({10.0, -1.0});
    EXPECT_DOUBLE_EQ(-1.0, result1.x);
    EXPECT_DOUBLE_EQ(0.0, result1.y);

    auto result2 = sph.ClosestNormal({3.0, -10.0});
    EXPECT_DOUBLE_EQ(0.0, result2.x);
    EXPECT_DOUBLE_EQ(1.0, result2.y);

    auto result3 = sph.ClosestNormal({3.0, 3.0});
    EXPECT_DOUBLE_EQ(0.0, result3.x);
    EXPECT_DOUBLE_EQ(-1.0, result3.y);
}


TEST(Sphere2, Builder) {
    Sphere2 sph = Sphere2::builder()
        .WithCenter({3.0, -1.0})
        .WithRadius(5.0)
        .Build();
    EXPECT_DOUBLE_EQ(3.0, sph.Center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph.Center.y);
    EXPECT_DOUBLE_EQ(5.0, sph.Radius);
}
