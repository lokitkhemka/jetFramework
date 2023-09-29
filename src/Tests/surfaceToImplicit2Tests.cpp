#include<Geometry/Box/box2.h>
#include<Geometry/Surface/surface_to_implicit2.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(SurfaceToImplicit2, Constructor) {
    auto box = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));

    SurfaceToImplicit2 s2i(box);
    EXPECT_EQ(box, s2i.Surface());

    s2i.IsNormalFlipped = true;
    SurfaceToImplicit2 s2i2(s2i);
    EXPECT_EQ(box, s2i2.Surface());
    EXPECT_TRUE(s2i2.IsNormalFlipped);
}

TEST(SurfaceToImplicit2, ClosestPoint) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxPoint = box->ClosestPoint(pt);
    Vector2D s2iPoint = s2i.ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, s2iPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, s2iPoint.y);
}

TEST(SurfaceToImplicit2, ClosestDistance) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);
}

TEST(SurfaceToImplicit2, Intersects) {
    auto box = std::make_shared<Box2>(BoundingBox2D({-1, 2}, {5, 3}));
    SurfaceToImplicit2 s2i(box);

    bool result0 = s2i.Intersects(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 = s2i.Intersects(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 = s2i.Intersects(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(SurfaceToImplicit2, ClosestIntersection) {
    auto box = std::make_shared<Box2>(BoundingBox2D({-1, 2}, {5, 3}));
    SurfaceToImplicit2 s2i(box);

    SurfaceRayIntersection2 result0 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0.IsIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(2), result0.t);
    EXPECT_EQ(Vector2D(0, 3), result0.Point);

    SurfaceRayIntersection2 result1 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1.IsIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.t);
    EXPECT_EQ(Vector2D(0.5, 2), result1.Point);

    SurfaceRayIntersection2 result2 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2.IsIntersecting);
}

TEST(SurfaceToImplicit2, BoundingBox) {
    auto box = std::make_shared<Box2>(BoundingBox2D({-1, 2}, {5, 3}));
    SurfaceToImplicit2 s2i(box);

    auto bbox = s2i.BoundingBox();
    EXPECT_DOUBLE_EQ(-1.0, bbox.LowerCorner.x);
    EXPECT_DOUBLE_EQ(2.0, bbox.LowerCorner.y);
    EXPECT_DOUBLE_EQ(5.0, bbox.UpperCorner.x);
    EXPECT_DOUBLE_EQ(3.0, bbox.UpperCorner.y);
}

TEST(SurfaceToImplicit2, SignedDistance) {
    BoundingBox2D bbox(Vector2D(1, 4), Vector2D(5, 6));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    SurfaceToImplicit2 s2i(box);

    Vector2D pt(-1, 7);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);

    s2i.IsNormalFlipped = true;
    s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(-boxDist, s2iDist);
}

TEST(SurfaceToImplicit2, ClosestNormal) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->IsNormalFlipped = true;

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxNormal = box->ClosestNormal(pt);
    Vector2D s2iNormal = s2i.ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, s2iNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, s2iNormal.y);
}