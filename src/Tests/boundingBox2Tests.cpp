#include<Geometry/BoundingBox/bounding_box2.h>
#include<gtest/gtest.h>
#include<limits>
#include<constants.h>

using namespace jet;

TEST(BoundingBox2, Constructors) {
    {
        BoundingBox2D box;

        EXPECT_DOUBLE_EQ(kMaxD, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(kMaxD, box.LowerCorner.y);

        EXPECT_DOUBLE_EQ(-kMaxD, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(-kMaxD, box.UpperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.y);

        EXPECT_DOUBLE_EQ(4.0, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.UpperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));
        BoundingBox2D box2(box);

        EXPECT_DOUBLE_EQ(-2.0, box2.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box2.LowerCorner.y);

        EXPECT_DOUBLE_EQ(4.0, box2.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box2.UpperCorner.y);
    }
}

TEST(BoundingBox2, BasicGetters) {
    BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

    EXPECT_DOUBLE_EQ(6.0, box.Width());
    EXPECT_DOUBLE_EQ(5.0, box.Height());
    EXPECT_DOUBLE_EQ(6.0, box.Length(0));
    EXPECT_DOUBLE_EQ(5.0, box.Length(1));
}

TEST(BoundingBox2, Overlaps) {
    // x-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(5.0, 1.0), Vector2D(8.0, 2.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 4.0), Vector2D(8.0, 6.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        EXPECT_TRUE(box1.Overlaps(box2));
    }
}

TEST(BoundingBox2, Contains) {
    // Not containing (x-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(-3.0, 0.0);

        EXPECT_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 3.5);

        EXPECT_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 0.0);

        EXPECT_TRUE(box.Contains(point));
    }
}

TEST(BoundingBox2, Intersects) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));

    Ray2D ray1(Vector2D(-3, 0), Vector2D(2, 1).Normalized());
    EXPECT_TRUE(box.Intersects(ray1));

    Ray2D ray2(Vector2D(3, -1), Vector2D(-1, 2).Normalized());
    EXPECT_TRUE(box.Intersects(ray2));

    Ray2D ray3(Vector2D(1, -5), Vector2D(2, 1).Normalized());
    EXPECT_FALSE(box.Intersects(ray3));
}

TEST(BoundingBox2, ClosestIntersection) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(1.0, 0.0));

    Ray2D ray1(Vector2D(-4, -3), Vector2D(1, 1).Normalized());
    BoundingBoxRayIntersection2D intersection1
        = box.ClosestIntersection(ray1);
    EXPECT_TRUE(intersection1.IsIntersecting);
    EXPECT_DOUBLE_EQ(Vector2D(2, 2).Length(), intersection1.tNear);
    EXPECT_DOUBLE_EQ(Vector2D(3, 3).Length(), intersection1.tFar);

    Ray2D ray2(Vector2D(0, -1), Vector2D(-2, 1).Normalized());
    BoundingBoxRayIntersection2D intersection2
        = box.ClosestIntersection(ray2);
    EXPECT_TRUE(intersection2.IsIntersecting);
    EXPECT_DOUBLE_EQ(Vector2D(2, 1).Length(), intersection2.tNear);
}

TEST(BoundingBox2, MidPoint) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    Vector2D midPoint = box.MidPoint();

    EXPECT_DOUBLE_EQ(1.0, midPoint.x);
    EXPECT_DOUBLE_EQ(0.5, midPoint.y);
}

TEST(BoundingBox2, DiagonalLength) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    double diagLen = box.DiagonalLength();

    EXPECT_DOUBLE_EQ(std::sqrt(6.0*6.0 + 5.0*5.0), diagLen);
}

TEST(BoundingBox2, DiagonalLengthSquared) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    double diagLenSqr = box.DiagonalLengthSquared();

    EXPECT_DOUBLE_EQ(6.0*6.0 + 5.0*5.0, diagLenSqr);
}

TEST(BoundingBox2, Reset) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Reset();

    static const double maxDouble = std::numeric_limits<double>::max();

    EXPECT_DOUBLE_EQ(maxDouble, box.LowerCorner.x);
    EXPECT_DOUBLE_EQ(maxDouble, box.LowerCorner.y);

    EXPECT_DOUBLE_EQ(-maxDouble, box.UpperCorner.x);
    EXPECT_DOUBLE_EQ(-maxDouble, box.UpperCorner.y);
}

TEST(BoundingBox2, Merge) {
    // Merge with point
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(5.0, 1.0);

        box.Merge(point);

        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.LowerCorner.y);

        EXPECT_DOUBLE_EQ(5.0, box.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.UpperCorner.y);
    }

    // Merge with other box
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        box1.Merge(box2);

        EXPECT_DOUBLE_EQ(-2.0, box1.LowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box1.LowerCorner.y);

        EXPECT_DOUBLE_EQ(8.0, box1.UpperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box1.UpperCorner.y);
    }
}

TEST(BoundingBox2, Expand) {
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Expand(3.0);

    EXPECT_DOUBLE_EQ(-5.0, box.LowerCorner.x);
    EXPECT_DOUBLE_EQ(-5.0, box.LowerCorner.y);

    EXPECT_DOUBLE_EQ(7.0, box.UpperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, box.UpperCorner.y);
}
