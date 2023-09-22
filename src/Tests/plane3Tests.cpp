#include<Geometry/Plane/plane3.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Plane3, Builder) {
    Plane3 plane = Plane3::builder()
        .WithNormal({1, 0, 0})
        .WithPoint({2, 3, 4})
        .Build();

    EXPECT_EQ(Vector3D(1, 0, 0), plane.Normal);
    EXPECT_EQ(Vector3D(2, 3, 4), plane.Point);
}