#include <Arrays/array1.h>
#include <NeighborhoodSearch/point3_parallel_hash_grid_search.h>
#include <gtest/gtest.h>
#include <vector>

using namespace jet;

TEST(PointParallelHashGridSearch3, ForEachNearbyPoint) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointParallelHashGridSearch3 searcher(4, 4, 4, std::sqrt(10));
    searcher.Build(points.Accessor());

    int cnt = 0;
    searcher.ForEachNearbyPoint(
        Vector3D(0, 0, 0),
        std::sqrt(10.0),
        [&](size_t i, const Vector3D& pt) {
            EXPECT_TRUE(i == 0 || i == 2);

            if (i == 0) {
                EXPECT_EQ(points[0], pt);
            } else if (i == 2) {
                EXPECT_EQ(points[2], pt);
            }

            ++cnt;
        });
    EXPECT_EQ(2, cnt);
}

TEST(PointParallelHashGridSearch3, ForEachNearbyPointEmpty) {
    Array1<Vector3D> points;

    PointParallelHashGridSearch3 searcher(4, 4, 4, std::sqrt(10));
    searcher.Build(points.Accessor());

    searcher.ForEachNearbyPoint(
        Vector3D(0, 0, 0),
        std::sqrt(10.0),
        [](size_t, const Vector3D&) {
        });
}

TEST(PointParallelHashGridSearch3, CopyConstructor) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointParallelHashGridSearch3 searcher(4, 4, 4, std::sqrt(10));
    searcher.Build(points.Accessor());

    PointParallelHashGridSearch3 searcher2(searcher);
    int cnt = 0;
    searcher2.ForEachNearbyPoint(
        Vector3D(0, 0, 0),
        std::sqrt(10.0),
        [&](size_t i, const Vector3D& pt) {
            EXPECT_TRUE(i == 0 || i == 2);

            if (i == 0) {
                EXPECT_EQ(points[0], pt);
            } else if (i == 2) {
                EXPECT_EQ(points[2], pt);
            }

            ++cnt;
        });
    EXPECT_EQ(2, cnt);
}

TEST(PointParallelHashGridSearch3, Serialization) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointParallelHashGridSearch3 searcher(4, 4, 4, std::sqrt(10));
    searcher.Build(points.Accessor());

    std::vector<uint8_t> buffer;
    searcher.Serialize(&buffer);

    PointParallelHashGridSearch3 searcher2(1, 1, 1, 1.0);
    searcher2.Deserialize(buffer);

    int cnt = 0;
    searcher2.ForEachNearbyPoint(
        Vector3D(0, 0, 0),
        std::sqrt(10.0),
        [&](size_t i, const Vector3D& pt) {
            EXPECT_TRUE(i == 0 || i == 2);

            if (i == 0) {
                EXPECT_EQ(points[0], pt);
            } else if (i == 2) {
                EXPECT_EQ(points[2], pt);
            }

            ++cnt;
        });
    EXPECT_EQ(2, cnt);
}