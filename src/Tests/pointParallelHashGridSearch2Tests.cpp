#include <Arrays/array1.h>
#include <NeighborhoodSearch/point2_parallel_hash_grid_search.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(PointParallelHashGridSearch2, ForEachNearbyPoint) {
    Array1<Vector2D> points = {
        Vector2D(1, 3),
        Vector2D(2, 5),
        Vector2D(-1, 3)
    };

    PointParallelHashGridSearch2 searcher(4, 4, std::sqrt(10));
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

TEST(PointParallelHashGridSearch2, ForEachNearbyPointEmpty) {
    Array1<Vector2D> points;

    PointParallelHashGridSearch2 searcher(4, 4, std::sqrt(10));
    searcher.Build(points.Accessor());

    searcher.ForEachNearbyPoint(
        Vector2D(0, 0),
        std::sqrt(10.0),
        [](size_t, const Vector2D&) {
        });
}