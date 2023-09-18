#include<Geometry/BoundingBox/bounding_box3.h>
#include<gtest/gtest.h>
#include<limits>

using namespace jet;

TEST(BoundingBox3, Constructors) {
    {
        BoundingBox3D box;

        EXPECT_DOUBLE_EQ(kMaxD, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(kMaxD, box.LowerCorner.y);
        EXPECT_DOUBLE_EQ(kMaxD, box.LowerCorner.z);

        EXPECT_DOUBLE_EQ(-kMaxD, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(-kMaxD, box.UpperCorner.y);
        EXPECT_DOUBLE_EQ(-kMaxD, box.UpperCorner.z);
    }

    {
        BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.y);
        EXPECT_DOUBLE_EQ(1.0, box.LowerCorner.z);

        EXPECT_DOUBLE_EQ(4.0, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.UpperCorner.y);
        EXPECT_DOUBLE_EQ(5.0, box.UpperCorner.z);
    }

    {
        BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));
        BoundingBox3D box2(box);

        EXPECT_DOUBLE_EQ(-2.0, box2.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box2.LowerCorner.y);
        EXPECT_DOUBLE_EQ(1.0, box2.LowerCorner.z);

        EXPECT_DOUBLE_EQ(4.0, box2.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box2.UpperCorner.y);
        EXPECT_DOUBLE_EQ(5.0, box2.UpperCorner.z);
    }
}

TEST(BoundingBox3, BasicGetters) {
    BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

    EXPECT_DOUBLE_EQ(6.0, box.Width());
    EXPECT_DOUBLE_EQ(5.0, box.Height());
    EXPECT_DOUBLE_EQ(4.0, box.Depth());
    EXPECT_DOUBLE_EQ(6.0, box.Length(0));
    EXPECT_DOUBLE_EQ(5.0, box.Length(1));
    EXPECT_DOUBLE_EQ(4.0, box.Length(2));
}

TEST(BoundingBox3, Overlaps) {
    // x-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(5.0, 1.0, 3.0), Vector3D(8.0, 2.0, 4.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 4.0, 3.0), Vector3D(8.0, 6.0, 4.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // z-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 6.0), Vector3D(8.0, 2.0, 9.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

        EXPECT_TRUE(box1.Overlaps(box2));
    }
}

TEST(BoundingBox3, Contains) {
    // Not containing (x-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(-3.0, 0.0, 4.0);

        EXPECT_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 3.5, 4.0);

        EXPECT_FALSE(box.Contains(point));
    }

    // Not containing (z-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 0.0, 0.0);

        EXPECT_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 0.0, 4.0);

        EXPECT_TRUE(box.Contains(point));
    }
}

TEST(BoundingBox3, Intersects) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));

    Ray3D ray1(Vector3D(-3, 0, 2), Vector3D(2, 1, 1).Normalized());
    EXPECT_TRUE(box.Intersects(ray1));

    Ray3D ray2(Vector3D(3, -1, 3), Vector3D(-1, 2, -3).Normalized());
    EXPECT_TRUE(box.Intersects(ray2));

    Ray3D ray3(Vector3D(1, -5, 1), Vector3D(2, 1, 2).Normalized());
    EXPECT_FALSE(box.Intersects(ray3));
}

TEST(BoundingBox3, ClosestIntersection) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, -1.0), Vector3D(1.0, 0.0, 1.0));

    Ray3D ray1(Vector3D(-4, -3, 0), Vector3D(1, 1, 0).Normalized());
    BoundingBoxRayIntersection3D intersection1
        = box.ClosestIntersection(ray1);
    EXPECT_TRUE(intersection1.IsIntersecting);
    EXPECT_DOUBLE_EQ(Vector3D(2, 2, 0).Length(), intersection1.tNear);
    EXPECT_DOUBLE_EQ(Vector3D(3, 3, 0).Length(), intersection1.tFar);

    Ray3D ray2(Vector3D(0, -1, 0), Vector3D(-2, 1, 1).Normalized());
    BoundingBoxRayIntersection3D intersection2
        = box.ClosestIntersection(ray2);
    EXPECT_TRUE(intersection2.IsIntersecting);
    EXPECT_DOUBLE_EQ(Vector3D(2, 1, 1).Length(), intersection2.tNear);
}

TEST(BoundingBox3, MidPoint) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    Vector3D midPoint = box.MidPoint();

    EXPECT_DOUBLE_EQ(1.0, midPoint.x);
    EXPECT_DOUBLE_EQ(0.5, midPoint.y);
    EXPECT_DOUBLE_EQ(3.0, midPoint.z);
}

TEST(BoundingBox3, DiagonalLength) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    double diagLen = box.DiagonalLength();

    EXPECT_DOUBLE_EQ(std::sqrt(6.0*6.0 + 5.0*5.0 + 4.0*4.0), diagLen);
}

TEST(BoundingBox3, DiagonalLengthSquared) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    double diagLenSqr = box.DiagonalLengthSquared();

    EXPECT_DOUBLE_EQ(6.0*6.0 + 5.0*5.0 + 4.0*4.0, diagLenSqr);
}

TEST(BoundingBox3, Reset) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    box.Reset();

    static const double maxDouble = std::numeric_limits<double>::max();

    EXPECT_DOUBLE_EQ(maxDouble, box.LowerCorner.x);
    EXPECT_DOUBLE_EQ(maxDouble, box.LowerCorner.y);
    EXPECT_DOUBLE_EQ(maxDouble, box.LowerCorner.z);

    EXPECT_DOUBLE_EQ(-maxDouble, box.UpperCorner.x);
    EXPECT_DOUBLE_EQ(-maxDouble, box.UpperCorner.y);
    EXPECT_DOUBLE_EQ(-maxDouble, box.UpperCorner.z);
}

TEST(BoundingBox3, Merge) {
    // Merge with point
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(5.0, 1.0, -1.0);

        box.Merge(point);

        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.y);
        EXPECT_DOUBLE_EQ(-1.0, box.LowerCorner.z);

        EXPECT_DOUBLE_EQ(5.0, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.UpperCorner.y);
        EXPECT_DOUBLE_EQ(5.0, box.UpperCorner.z);
    }

    // Merge with other box
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

        box1.Merge(box2);

        EXPECT_DOUBLE_EQ(-2.0, box1.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box1.LowerCorner.y);
        EXPECT_DOUBLE_EQ(1.0, box1.LowerCorner.z);

        EXPECT_DOUBLE_EQ(8.0, box1.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box1.UpperCorner.y);
        EXPECT_DOUBLE_EQ(7.0, box1.UpperCorner.z);
    }
}

TEST(BoundingBox3, Expand) {
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    box.Expand(3.0);

    EXPECT_DOUBLE_EQ(-5.0, box.LowerCorner.x);
    EXPECT_DOUBLE_EQ(-5.0, box.LowerCorner.y);
    EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.z);

    EXPECT_DOUBLE_EQ(7.0, box.UpperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, box.UpperCorner.y);
    EXPECT_DOUBLE_EQ(8.0, box.UpperCorner.z);
}