#include<Points/point2.h>
#include <gtest/gtest.h>

TEST(Point2, Constructors)
{
    jet::Point2F pt;
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(0.f, pt.y);

    jet::Point2F pt2(5.f, 3.f);
    EXPECT_FLOAT_EQ(5.f, pt2.x);
    EXPECT_FLOAT_EQ(3.f, pt2.y);

    jet::Point2F pt5 = { 7.f, 6.f };
    EXPECT_FLOAT_EQ(7.f, pt5.x);
    EXPECT_FLOAT_EQ(6.f, pt5.y);

    jet::Point2F pt6(pt5);
    EXPECT_FLOAT_EQ(7.f, pt6.x);
    EXPECT_FLOAT_EQ(6.f, pt6.y);
}

TEST(Point2, SetMethods)
{
    jet::Point2F pt;
    pt.Set(4.f, 2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(2.f, pt.y);

    auto lst = {0.f, 5.f};
    pt.Set(lst);
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(5.f, pt.y);

    pt.Set(jet::Point2F(9.f, 8.f));
    EXPECT_FLOAT_EQ(9.f, pt.x);
    EXPECT_FLOAT_EQ(8.f, pt.y);
}

TEST(Point2, BasicSetterMethods)
{
    jet::Point2F pt(3.f, 9.f);
    pt.SetZero();
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(0.f, pt.y);
}

TEST(Point2, BinaryOperatorMethods) {
    jet::Point2F pt(3.f, 9.f);
    pt = pt.Add(4.f);
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(13.f, pt.y);

    pt = pt.Add(jet::Point2F(-2.f, 1.f));
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(14.f, pt.y);

    pt = pt.Sub(8.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt.Sub(jet::Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt = pt.Mul(2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt.Mul(jet::Point2F(3.f, -2.f));
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt = pt.Div(4.f);
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt = pt.Div(jet::Point2F(3.f, -1.f));
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, BinaryInverseOperatorMethods) {
    jet::Point2F pt(3.f, 9.f);
    pt = pt.RSub(8.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(-1.f, pt.y);

    pt = pt.RSub(jet::Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(-10.f, pt.x);
    EXPECT_FLOAT_EQ(4.f, pt.y);

    pt = jet::Point2F(-4.f, -3.f);
    pt = pt.RDiv(12.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, -4.f);

    pt = pt.RDiv(jet::Point2F(3.f, -16.f));
    EXPECT_FLOAT_EQ(-1.f, pt.x);
    EXPECT_FLOAT_EQ(4.f, pt.y);
}

TEST(Point2, AugmentedOperatorMethods) {
    jet::Point2F pt(3.f, 9.f);
    pt.IAdd(4.f);
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt.IAdd(jet::Point2F(-2.f, 1.f));
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt.ISub(8.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt.ISub(jet::Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt.IMul(2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt.IMul(jet::Point2F(3.f, -2.f));
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt.IDiv(4.f);
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt.IDiv(jet::Point2F(3.f, -1.f));
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, AtMethod) {
    jet::Point2F pt(8.f, 9.f);
    EXPECT_FLOAT_EQ(pt.At(0), 8.f);
    EXPECT_FLOAT_EQ(pt.At(1), 9.f);

    pt.At(0) = 7.f;
    pt.At(1) = 6.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);
}

TEST(Point2, BasicGetterMethods) {
    jet::Point2F pt(3.f, 7.f), pt2(-3.f, -7.f);

    float sum = pt.Sum();
    EXPECT_FLOAT_EQ(sum, 10.f);

    float min = pt.Min();
    EXPECT_FLOAT_EQ(min, 3.f);

    float max = pt.Max();
    EXPECT_FLOAT_EQ(max, 7.f);

    float absmin = pt2.AbsMin();
    EXPECT_FLOAT_EQ(absmin, -3.f);

    float absmax = pt2.AbsMax();
    EXPECT_FLOAT_EQ(absmax, -7.f);

    size_t daxis = pt.DominantAxis();
    EXPECT_EQ(daxis, (size_t)1);

    size_t saxis = pt.SubminantAxis();
    EXPECT_EQ(saxis, (size_t)0);
}


TEST(Point2, BracketOperator) {
    jet::Point2F pt(8.f, 9.f);
    EXPECT_FLOAT_EQ(pt[0], 8.f);
    EXPECT_FLOAT_EQ(pt[1], 9.f);

    pt[0] = 7.f;
    pt[1] = 6.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);
}

TEST(Point2, AssignmentOperator) {
    jet::Point2F pt(5.f, 1.f);
    jet::Point2F pt2(3.f, 3.f);
    pt2 = pt;
    EXPECT_FLOAT_EQ(5.f, pt2.x);
    EXPECT_FLOAT_EQ(pt2.y, 1.f);
}

TEST(Point2, AugmentedOperators) {
    jet::Point2F pt(3.f, 9.f);
    pt += 4.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt += jet::Point2F(-2.f, 1.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt -= 8.f;
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt -= jet::Point2F(-5.f, 3.f);
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt *= 2.f;
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt *= jet::Point2F(3.f, -2.f);
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt /= 4.f;
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt /= jet::Point2F(3.f, -1.f);
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, EqualOperator) {
    jet::Point2F pt, pt2(3.f, 7.f), pt3(3.f, 5.f), pt4(5.f, 1.f);
    pt = pt2;
    EXPECT_TRUE(pt == pt2);
    EXPECT_FALSE(pt == pt3);
    EXPECT_FALSE(pt != pt2);
    EXPECT_TRUE(pt != pt3);
    EXPECT_TRUE(pt != pt4);
}

TEST(Point2, MinMaxFunction) {
    jet::Point2F pt(5.f, 1.f);
    jet::Point2F pt2(3.f, 3.f);
    jet::Point2F minPoint = Min(pt, pt2);
    jet::Point2F maxPoint = Max(pt, pt2);
    EXPECT_EQ(jet::Point2F(3.f, 1.f), minPoint);
    EXPECT_EQ(jet::Point2F(5.f, 3.f), maxPoint);
}

TEST(Point2, ClampFunction) {
    jet::Point2F pt(2.f, 4.f), low(3.f, -1.f), high(5.f, 2.f);
    jet::Point2F clampedVec = Clamp(pt, low, high);
    EXPECT_EQ(jet::Point2F(3.f, 2.f), clampedVec);
}

TEST(Point2, CeilFloorFunction) {
    jet::Point2F pt(2.2f, 4.7f);
    jet::Point2F ceilVec = Ceil(pt);
    EXPECT_EQ(jet::Point2F(3.f, 5.f), ceilVec);

    jet::Point2F floorVec = Floor(pt);
    EXPECT_EQ(jet::Point2F(2.f, 4.f), floorVec);
}

TEST(Point2, BinaryOperators) {
    jet::Point2F pt(3.f, 9.f);
    pt = pt + 4.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt = pt + jet::Point2F(-2.f, 1.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt = pt - 8.f;
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt - jet::Point2F(-5.f, 3.f);
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt = pt * 2.f;
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt * jet::Point2F(3.f, -2.f);
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt = pt / 4.f;
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt = pt / jet::Point2F(3.f, -1.f);
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

