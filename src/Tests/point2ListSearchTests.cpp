#include <Arrays/array1.h>
#include <NeighborhoodSearch/point2_list search.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(PointListSearch2, ForEachNearbyPoint) {
    Array1<Vector2D> points = {
        Vector2D(1, 3),
        Vector2D(2, 5),
        Vector2D(-1, 3)
    };

    PointListSearch2 searcher;
    searcher.Build(points.Accessor());

    searcher.ForEachNearbyPoint(
        Vector2D(0, 0),
        std::sqrt(10.0),
        [&](size_t i, const Vector2D& pt) {
            EXPECT_TRUE(i == 0 || i == 2);

            if (i == 0) {
                EXPECT_EQ(points[0], pt);
            } else if (i == 2) {
                EXPECT_EQ(points[2], pt);
            }
        });
}