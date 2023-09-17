#include<Geometry/Plane/plane2.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Plane2, Builder)
{
    Plane2 plane = Plane2::Builder().WithNormal({1,0})
                        .WithPoint({2,3}).Build();

    EXPECT_EQ(Vector2D(1, 0), plane.Normal);
    EXPECT_EQ(Vector2D(2, 3), plane.Point);
}