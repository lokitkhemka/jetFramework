#include <Geometry/Box/box2.h>
#include <Geometry/ImplicitSurface/implicit_surface2_set.h>
#include <Geometry/Surface/surface_to_implicit2.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(ImplicitSurfaceSet2, Constructor) {
    ImplicitSurfaceSet2 sset;
    EXPECT_EQ(0u, sset.NumberOfSurfaces());

    sset.IsNormalFlipped = true;
    auto box = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));
    sset.AddExplicitSurface(box);

    ImplicitSurfaceSet2 sset2(sset);
    EXPECT_EQ(1u, sset2.NumberOfSurfaces());
    EXPECT_TRUE(sset2.IsNormalFlipped);
}

TEST(ImplicitSurfaceSet2, NumberOfSurfaces) {
    ImplicitSurfaceSet2 sset;

    auto box = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));
    sset.AddExplicitSurface(box);

    EXPECT_EQ(1u, sset.NumberOfSurfaces());
}

TEST(ImplicitSurfaceSet2, SurfaceAt) {
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({3, 4}, {5, 6}));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto implicitSurfaceAt0
        = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1
        = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

    EXPECT_EQ(
        std::dynamic_pointer_cast<Surface2>(box1),
        implicitSurfaceAt0->Surface());
    EXPECT_EQ(
        std::dynamic_pointer_cast<Surface2>(box2),
        implicitSurfaceAt1->Surface());
}

TEST(ImplicitSurfaceSet2, AddSurface) {
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({3, 4}, {5, 6}));
    auto implicitBox = std::make_shared<SurfaceToImplicit2>(box2);

    sset.AddExplicitSurface(box1);
    sset.AddSurface(implicitBox);

    EXPECT_EQ(2u, sset.NumberOfSurfaces());

    auto implicitSurfaceAt0
        = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1
        = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

    EXPECT_EQ(
        std::dynamic_pointer_cast<Surface2>(box1),
        implicitSurfaceAt0->Surface());
    EXPECT_EQ(
        implicitBox,
        implicitSurfaceAt1);
}

TEST(ImplicitSurfaceSet2, ClosestPoint) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->IsNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxPoint = box->ClosestPoint(pt);
    Vector2D setPoint = sset->ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, setPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, setPoint.y);
}

TEST(ImplicitSurfaceSet2, ClosestDistance) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->IsNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    double boxDist = box->ClosestDistance(pt);
    double setDist = sset->ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet2, Intersects) {
    ImplicitSurfaceSet2 sset;
    auto box = std::make_shared<Box2>(BoundingBox2D({-1, 2}, {5, 3}));
    sset.AddExplicitSurface(box);

    bool result0 = sset.Intersects(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 = sset.Intersects(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 = sset.Intersects(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(ImplicitSurfaceSet2, ClosestIntersection) {
    ImplicitSurfaceSet2 sset;
    auto box = std::make_shared<Box2>(BoundingBox2D({-1, 2}, {5, 3}));
    sset.AddExplicitSurface(box);

    SurfaceRayIntersection2 result0 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0.IsIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(2), result0.t);
    EXPECT_EQ(Vector2D(0, 3), result0.Point);

    SurfaceRayIntersection2 result1 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1.IsIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.t);
    EXPECT_EQ(Vector2D(0.5, 2), result1.Point);

    SurfaceRayIntersection2 result2 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2.IsIntersecting);
}

TEST(ImplicitSurfaceSet2, BoundingBox) {
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({0, 0}, {1, 2}));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({3, 4}, {5, 6}));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto bbox = sset.BoundingBox();
    EXPECT_DOUBLE_EQ(0.0, bbox.LowerCorner.x);
    EXPECT_DOUBLE_EQ(0.0, bbox.LowerCorner.y);
    EXPECT_DOUBLE_EQ(5.0, bbox.UpperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, bbox.UpperCorner.y);
}

TEST(ImplicitSurfaceSet2, SignedDistance) {
    BoundingBox2D bbox(Vector2D(1, 4), Vector2D(5, 6));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    auto implicitBox = std::make_shared<SurfaceToImplicit2>(box);

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(-1, 7);
    double boxDist = implicitBox->SignedDistance(pt);
    double setDist = sset->SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet2, ClosestNormal) {
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->IsNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxNormal = box->ClosestNormal(pt);
    Vector2D setNormal = sset->ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, setNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, setNormal.y);
}