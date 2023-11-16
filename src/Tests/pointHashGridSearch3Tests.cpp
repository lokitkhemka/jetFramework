#include <Arrays/array1.h>
#include <Arrays/array3.h>
#include <Geometry/PointGenerator/bcc_lattice_point_generator.h>
#include <Geometry/BoundingBox/bounding_box3.h>
#include <NeighborhoodSearch/point3_hash_grid_search.h>
#include <NeighborhoodSearch/point3_parallel_hash_grid_search.h>
#include <gtest/gtest.h>
#include <vector>

using namespace jet;

TEST(PointHashGridSearch3, ForEachNearbyPoint) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointHashGridSearch3 searcher(4, 4, 4, 2.0 * std::sqrt(10));
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

TEST(PointHashGridSearch3, ForEachNearbyPointEmpty) {
    Array1<Vector3D> points;

    PointHashGridSearch3 searcher(4, 4, 4, 2.0 * std::sqrt(10));
    searcher.Build(points.Accessor());

    searcher.ForEachNearbyPoint(
        Vector3D(0, 0, 0),
        std::sqrt(10.0),
        [](size_t, const Vector3D&) {
        });
}

TEST(PointParallelHashGridSearcher3, Build) {
    Array1<Vector3D> points;
    BCCLatticePointGenerator pointsGenerator;
    BoundingBox3D bbox(
        Vector3D(0, 0, 0),
        Vector3D(1, 1, 1));
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointHashGridSearch3 pointSearcher(4, 4, 4, 0.18);
    pointSearcher.Build(points);

    Array3<size_t> grid(4, 4, 4);

    grid.ForEachIndex([&](size_t i, size_t j, size_t k) {
        size_t key = pointSearcher.GetHashKeyFromBucketIndex(
            Point3I(
                static_cast<ssize_t>(i),
                static_cast<ssize_t>(j),
                static_cast<ssize_t>(k)));
        size_t value = pointSearcher.Buckets()[key].size();
        grid(i, j, k) = value;
    });

    PointParallelHashGridSearch3 parallelSearcher(4, 4, 4, 0.18);
    parallelSearcher.Build(points);

    grid.ForEachIndex([&](size_t i, size_t j, size_t k) {
        size_t key = parallelSearcher.GetHashKeyFromBucketIndex(
            Point3I(
                static_cast<ssize_t>(i),
                static_cast<ssize_t>(j),
                static_cast<ssize_t>(k)));
        size_t start = parallelSearcher.StartIndexTable()[key];
        size_t end = parallelSearcher.EndIndexTable()[key];
        size_t value = end - start;
        EXPECT_EQ(grid(i, j, k), value);
    });
}

TEST(PointHashGridSearch3, CopyConstructor) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointHashGridSearch3 searcher(4, 4, 4, 2.0 * std::sqrt(10));
    searcher.Build(points.Accessor());

    PointHashGridSearch3 searcher2(searcher);
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

TEST(PointHashGridSearch3, Serialize) {
    Array1<Vector3D> points = {
        Vector3D(0, 1, 3),
        Vector3D(2, 5, 4),
        Vector3D(-1, 3, 0)
    };

    PointHashGridSearch3 searcher(4, 4, 4, 2.0 * std::sqrt(10));
    searcher.Build(points.Accessor());

    std::vector<uint8_t> buffer;
    searcher.Serialize(&buffer);

    PointHashGridSearch3 searcher2(1, 1, 1, 1.0);
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