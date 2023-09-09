#include<Geometry/Ray/ray2.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Ray2, Constructors) {
    Ray2D ray;
    EXPECT_EQ(Vector2D(), ray.Origin);
    EXPECT_EQ(Vector2D(1, 0), ray.Direction);

    Ray2D ray2({1, 2}, {3, 4});
    EXPECT_EQ(Vector2D(1, 2), ray2.Origin);
    EXPECT_EQ(Vector2D(3, 4).Normalized(), ray2.Direction);

    Ray2D ray3(ray2);
    EXPECT_EQ(Vector2D(1, 2), ray3.Origin);
    EXPECT_EQ(Vector2D(3, 4).Normalized(), ray3.Direction);
}

TEST(Ray2, PointAt) {
    Ray2D ray;
    EXPECT_EQ(Vector2D(4.5, 0.0), ray.PointAt(4.5));
}