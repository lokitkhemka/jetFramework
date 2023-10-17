#include <Arrays/array1.h>
#include <Arrays/array2.h>
#include <Geometry/BoundingBox/bounding_box2.h>
#include <NeighborhoodSearch/point2_hash_grid_search.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(PointHashGridSearch2, ForEachNearbyPoint) {
    Array1<Vector2D> points = {
        Vector2D(1, 3),
        Vector2D(2, 5),
        Vector2D(-1, 3)
    };

    PointHashGridSearch2 searcher(4, 4, 2.0 * std::sqrt(10));
    searcher.Build(points.Accessor());

    searcher.ForEachNearbyPoint(
        Vector2D(0, 0),
        std::sqrt(10.0),
        [&points](size_t i, const Vector2D& pt) {
            EXPECT_TRUE(i == 0 || i == 2);

            if (i == 0) {
                EXPECT_EQ(points[0], pt);
            } else if (i == 2) {
                EXPECT_EQ(points[2], pt);
            }
        });
}