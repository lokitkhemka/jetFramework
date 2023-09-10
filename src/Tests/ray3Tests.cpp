#include<Geometry/Ray/ray3.h>
#include<gtest/gtest.h>


using namespace jet;

TEST(Ray3, Constructors) {
    Ray3D ray;
    EXPECT_EQ(Vector3D(), ray.Origin);
    EXPECT_EQ(Vector3D(1, 0, 0), ray.Direction);

    Ray3D ray2({1, 2, 3}, {4, 5, 6});
    EXPECT_EQ(Vector3D(1, 2, 3), ray2.Origin);
    EXPECT_EQ(Vector3D(4, 5, 6).Normalized(), ray2.Direction);

    Ray3D ray3(ray2);
    EXPECT_EQ(Vector3D(1, 2, 3), ray3.Origin);
    EXPECT_EQ(Vector3D(4, 5, 6).Normalized(), ray3.Direction);
}

TEST(Ray3, PointAt) {
    Ray3D ray;
    EXPECT_EQ(Vector3D(4.5, 0.0, 0.0), ray.PointAt(4.5));
}