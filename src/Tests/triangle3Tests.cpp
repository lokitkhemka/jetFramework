#include<Geometry/TriangleMesh/triangle3.h>
#include<gtest/gtest.h>
#include "unit_test_utils.h"

using namespace jet;

TEST(Triangle3, Constructors) {
    Triangle3 tri1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(0.0, tri1.Points[i][j]);
            EXPECT_DOUBLE_EQ(0.0, tri1.Normals[i][j]);
        }
        for (int j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(0.0, tri1.UVs[i][j]);
        }
    }

    std::array<Vector3D, 3> points = {
            {Vector3D(1, 2, 3), Vector3D(4, 5, 6), Vector3D(7, 8, 9)}};
    std::array<Vector3D, 3> normals = {
            {Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1)}};
    std::array<Vector2D, 3> uvs = {
            {Vector2D(1, 0), Vector2D(0, 1), Vector2D(0.5, 0.5)}};

    Triangle3 tri2(points, normals, uvs);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(points[i][j], tri2.Points[i][j]);
            EXPECT_DOUBLE_EQ(normals[i][j], tri2.Normals[i][j]);
        }
        for (int j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(uvs[i][j], tri2.UVs[i][j]);
        }
    }

    Triangle3 tri3(tri2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(points[i][j], tri3.Points[i][j]);
            EXPECT_DOUBLE_EQ(normals[i][j], tri3.Normals[i][j]);
        }
        for (int j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(uvs[i][j], tri3.UVs[i][j]);
        }
    }
}

TEST(Triangle3, BasicGetters) {
    Triangle3 tri;
    tri.Points = {{Vector3D(0, 0, -1), Vector3D(1, 0, -1), Vector3D(0, 1, -1)}};

    EXPECT_DOUBLE_EQ(0.5, tri.Area());

    double b0, b1, b2;
    tri.GetBarycentricCoords(Vector3D(0.5, 0.5, -1), &b0, &b1, &b2);
    EXPECT_DOUBLE_EQ(0.0, b0);
    EXPECT_DOUBLE_EQ(0.5, b1);
    EXPECT_DOUBLE_EQ(0.5, b2);

    Vector3D n = tri.FaceNormal();
    EXPECT_VECTOR3_EQ(Vector3D(0, 0, 1), n);
}

TEST(Triangle3, SurfaceGetters) {
    Triangle3 tri;
    tri.Points = {{Vector3D(0, 0, -1), Vector3D(1, 0, -1), Vector3D(0, 1, -1)}};
    tri.Normals = {{Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1)}};
    Vector3D cp1 = tri.ClosestPoint({0.4, 0.4, 3.0});
    EXPECT_VECTOR3_EQ(Vector3D(0.4, 0.4, -1), cp1);

    Vector3D cp2 = tri.ClosestPoint({-3.0, -3.0, 0.0});
    EXPECT_VECTOR3_EQ(Vector3D(0, 0, -1), cp2);

    Vector3D cn1 = tri.ClosestNormal({0.4, 0.4, 3.0});
    EXPECT_VECTOR3_EQ(Vector3D(1, 2, 2).Normalized(), cn1);

    Vector3D cn2 = tri.ClosestNormal({-3.0, -3.0, 0.0});
    EXPECT_VECTOR3_EQ(Vector3D(1, 0, 0), cn2);

    bool ints1 = tri.Intersects(Ray3D({0.4, 0.4, -5.0}, {0, 0, 1}));
    EXPECT_TRUE(ints1);

    bool ints2 = tri.Intersects(Ray3D({-1, 2, 3}, {0, 0, -1}));
    EXPECT_FALSE(ints2);

    bool ints3 = tri.Intersects(Ray3D({1, 1, 0}, {0, 0, -1}));
    EXPECT_FALSE(ints3);

    auto cints1 = tri.ClosestIntersection(Ray3D({0.4, 0.4, -5.0}, {0, 0, 1}));
    EXPECT_TRUE(cints1.IsIntersecting);
    EXPECT_VECTOR3_EQ(Vector3D(0.4, 0.4, -1), cints1.Point);
    EXPECT_DOUBLE_EQ(4.0, cints1.t);
    EXPECT_VECTOR3_EQ(Vector3D(1, 2, 2).Normalized(), cints1.Normal);
}

TEST(Triangle3, Builder) {
    std::array<Vector3D, 3> points = {
            {Vector3D(1, 2, 3), Vector3D(4, 5, 6), Vector3D(7, 8, 9)}};
    std::array<Vector3D, 3> normals = {
            {Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1)}};
    std::array<Vector2D, 3> uvs = {
            {Vector2D(1, 0), Vector2D(0, 1), Vector2D(0.5, 0.5)}};

    Triangle3 tri = Triangle3::builder()
            .WithPoints(points)
            .WithNormals(normals)
            .WithUVs(uvs)
            .Build();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(points[i][j], tri.Points[i][j]);
            EXPECT_DOUBLE_EQ(normals[i][j], tri.Normals[i][j]);
        }
        for (int j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(uvs[i][j], tri.UVs[i][j]);
        }
    }
}
