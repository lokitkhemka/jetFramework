#include <Vector/vector4.h>
#include<gtest/gtest.h>
#include<algorithm>

using namespace jet;

TEST(Vector4, Constructors) {
    Vector4F vec;
    EXPECT_FLOAT_EQ(0.f, vec.x);
    EXPECT_FLOAT_EQ(0.f, vec.y);
    EXPECT_FLOAT_EQ(0.f, vec.z);
    EXPECT_FLOAT_EQ(0.f, vec.w);

    Vector4F vec2(5.f, 3.f, 8.f, 8.f);
    EXPECT_FLOAT_EQ(5.f, vec2.x);
    EXPECT_FLOAT_EQ(3.f, vec2.y);
    EXPECT_FLOAT_EQ(8.f, vec2.z);
    EXPECT_FLOAT_EQ(8.f, vec2.w);

    Vector3F vec3(4.f, 7.f, 8.f);
    Vector4F vec4(vec3, 9.f);
    EXPECT_FLOAT_EQ(4.f, vec4.x);
    EXPECT_FLOAT_EQ(7.f, vec4.y);
    EXPECT_FLOAT_EQ(8.f, vec4.z);
    EXPECT_FLOAT_EQ(9.f, vec4.w);

    Vector4F vec5 = { 7.f, 6.f, 1.f, 1.f };
    EXPECT_FLOAT_EQ(7.f, vec5.x);
    EXPECT_FLOAT_EQ(6.f, vec5.y);
    EXPECT_FLOAT_EQ(1.f, vec5.z);
    EXPECT_FLOAT_EQ(1.f, vec5.w);

    Vector4F vec6(vec5);
    EXPECT_FLOAT_EQ(7.f, vec6.x);
    EXPECT_FLOAT_EQ(6.f, vec6.y);
    EXPECT_FLOAT_EQ(1.f, vec6.z);
    EXPECT_FLOAT_EQ(1.f, vec6.w);
}

TEST(Vector4, SetMethods) {
    Vector4F vec;
    vec.Set(4.f, 2.f, 8.f, 8.f);
    EXPECT_FLOAT_EQ(4.f, vec.x);
    EXPECT_FLOAT_EQ(2.f, vec.y);
    EXPECT_FLOAT_EQ(8.f, vec.z);
    EXPECT_FLOAT_EQ(8.f, vec.w);

    vec.Set(Vector3F(1.f, 3.f, 10.f), 10.f);
    EXPECT_FLOAT_EQ(1.f, vec.x);
    EXPECT_FLOAT_EQ(3.f, vec.y);
    EXPECT_FLOAT_EQ(10.f, vec.z);
    EXPECT_FLOAT_EQ(10.f, vec.w);

    auto lst = {0.f, 5.f, 6.f, 6.f};
    vec.Set(lst);
    EXPECT_FLOAT_EQ(0.f, vec.x);
    EXPECT_FLOAT_EQ(5.f, vec.y);
    EXPECT_FLOAT_EQ(6.f, vec.z);
    EXPECT_FLOAT_EQ(6.f, vec.w);

    vec.Set(Vector4F(9.f, 8.f, 2.f, 2.f));
    EXPECT_FLOAT_EQ(9.f, vec.x);
    EXPECT_FLOAT_EQ(8.f, vec.y);
    EXPECT_FLOAT_EQ(2.f, vec.z);
    EXPECT_FLOAT_EQ(2.f, vec.w);
}


TEST(Vector4, BasicSetterMethods) {
    Vector4F vec(3.f, 9.f, 4.f, 4.f);
    vec.SetZero();
    EXPECT_FLOAT_EQ(0.f, vec.x);
    EXPECT_FLOAT_EQ(0.f, vec.y);
    EXPECT_FLOAT_EQ(0.f, vec.z);
    EXPECT_FLOAT_EQ(0.f, vec.w);

    vec.Set(4.f, 2.f, 8.f, 8.f);
    vec.Normalize();
    float len = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
    EXPECT_TRUE(fabsf(len - 1.f) < 1e-6);
}

TEST(Vector4, BinaryOperatorMethods) {
    Vector4F vec(3.f, 9.f, 4.f, 4.f);
    vec = vec.Add(4.f);
    EXPECT_FLOAT_EQ(7.f, vec.x);
    EXPECT_FLOAT_EQ(13.f, vec.y);
    EXPECT_FLOAT_EQ(8.f, vec.z);
    EXPECT_FLOAT_EQ(8.f, vec.w);

    vec = vec.Add(Vector4F(-2.f, 1.f, 5.f, 5.f));
    EXPECT_FLOAT_EQ(5.f, vec.x);
    EXPECT_FLOAT_EQ(14.f, vec.y);
    EXPECT_FLOAT_EQ(13.f, vec.z);
    EXPECT_FLOAT_EQ(13.f, vec.w);

    vec = vec.Sub(8.f);
    EXPECT_FLOAT_EQ(-3.f, vec.x);
    EXPECT_FLOAT_EQ(6.f, vec.y);
    EXPECT_FLOAT_EQ(5.f, vec.z);
    EXPECT_FLOAT_EQ(5.f, vec.w);

    vec = vec.Sub(Vector4F(-5.f, 3.f, 12.f, 12.f));
    EXPECT_FLOAT_EQ(2.f, vec.x);
    EXPECT_FLOAT_EQ(3.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);

    vec = vec.Mul(2.f);
    EXPECT_FLOAT_EQ(4.f, vec.x);
    EXPECT_FLOAT_EQ(6.f, vec.y);
    EXPECT_FLOAT_EQ(-14.f, vec.z);
    EXPECT_FLOAT_EQ(-14.f, vec.w);

    vec = vec.Mul(Vector4F(3.f, -2.f, 0.5f, 0.5f));
    EXPECT_FLOAT_EQ(12.f, vec.x);
    EXPECT_FLOAT_EQ(-12.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);

    vec = vec.Div(4.f);
    EXPECT_FLOAT_EQ(3.f, vec.x);
    EXPECT_FLOAT_EQ(-3.f, vec.y);
    EXPECT_FLOAT_EQ(-1.75f, vec.z);
    EXPECT_FLOAT_EQ(-1.75f, vec.w);

    vec = vec.Div(Vector4F(3.f, -1.f, 0.25f, 0.25f));
    EXPECT_FLOAT_EQ(1.f, vec.x);
    EXPECT_FLOAT_EQ(3.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);

    float d = vec.Dot(Vector4F(4.f, 2.f, 1.f, 1.f));
    EXPECT_FLOAT_EQ(-4.f, d);
}

TEST(Vector4, BinaryInverseOperatorMethods) {
    Vector4F vec(5.f, 14.f, 13.f, 13.f);
    vec = vec.RSub(8.f);
    EXPECT_FLOAT_EQ(3.f, vec.x);
    EXPECT_FLOAT_EQ(-6.f, vec.y);
    EXPECT_FLOAT_EQ(-5.f, vec.z);
    EXPECT_FLOAT_EQ(-5.f, vec.w);

    vec = vec.RSub(Vector4F(-5.f, 3.f, -1.f, -1.f));
    EXPECT_FLOAT_EQ(-8.f, vec.x);
    EXPECT_FLOAT_EQ(9.f, vec.y);
    EXPECT_FLOAT_EQ(4.f, vec.z);
    EXPECT_FLOAT_EQ(4.f, vec.w);

    vec = Vector4F(-12.f, -9.f, 8.f, 8.0f);
    vec = vec.RDiv(36.f);
    EXPECT_FLOAT_EQ(-3.f, vec.x);
    EXPECT_FLOAT_EQ(-4.f, vec.y);
    EXPECT_FLOAT_EQ(4.5f, vec.z);
    EXPECT_FLOAT_EQ(4.5f, vec.w);

    vec = vec.RDiv(Vector4F(3.f, -16.f, 18.f, 18.f));
    EXPECT_FLOAT_EQ(-1.f, vec.x);
    EXPECT_FLOAT_EQ(4.f, vec.y);
    EXPECT_FLOAT_EQ(4.f, vec.z);
    EXPECT_FLOAT_EQ(4.f, vec.w);
}

TEST(Vector4, AugmentedOperatorMethods) {
    Vector4F vec(3.f, 9.f, 4.f, 4.f);
    vec.IAdd(4.f);
    EXPECT_FLOAT_EQ(7.f, vec.x);
    EXPECT_FLOAT_EQ(13.f, vec.y);
    EXPECT_FLOAT_EQ(8.f, vec.z);
    EXPECT_FLOAT_EQ(8.f, vec.w);

    vec.IAdd(Vector4F(-2.f, 1.f, 5.f, 5.f));
    EXPECT_FLOAT_EQ(5.f, vec.x);
    EXPECT_FLOAT_EQ(14.f, vec.y);
    EXPECT_FLOAT_EQ(13.f, vec.z);
    EXPECT_FLOAT_EQ(13.f, vec.w);

    vec.ISub(8.f);
    EXPECT_FLOAT_EQ(-3.f, vec.x);
    EXPECT_FLOAT_EQ(6.f, vec.y);
    EXPECT_FLOAT_EQ(5.f, vec.z);
    EXPECT_FLOAT_EQ(5.f, vec.w);

    vec.ISub(Vector4F(-5.f, 3.f, 12.f, 12.f));
    EXPECT_FLOAT_EQ(2.f, vec.x);
    EXPECT_FLOAT_EQ(3.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);

    vec.IMul(2.f);
    EXPECT_FLOAT_EQ(4.f, vec.x);
    EXPECT_FLOAT_EQ(6.f, vec.y);
    EXPECT_FLOAT_EQ(-14.f, vec.z);
    EXPECT_FLOAT_EQ(-14.f, vec.w);

    vec.IMul(Vector4F(3.f, -2.f, 0.5f, 0.5f));
    EXPECT_FLOAT_EQ(12.f, vec.x);
    EXPECT_FLOAT_EQ(-12.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);

    vec.IDiv(4.f);
    EXPECT_FLOAT_EQ(3.f, vec.x);
    EXPECT_FLOAT_EQ(-3.f, vec.y);
    EXPECT_FLOAT_EQ(-1.75f, vec.z);
    EXPECT_FLOAT_EQ(-1.75f, vec.w);

    vec.IDiv(Vector4F(3.f, -1.f, 0.25f, 0.25f));
    EXPECT_FLOAT_EQ(1.f, vec.x);
    EXPECT_FLOAT_EQ(3.f, vec.y);
    EXPECT_FLOAT_EQ(-7.f, vec.z);
    EXPECT_FLOAT_EQ(-7.f, vec.w);
}

TEST(Vector4, AtMethods) {
    Vector4F vec(8.f, 9.f, 1.f, 1.f);
    EXPECT_FLOAT_EQ(8.f, vec.At(0));
    EXPECT_FLOAT_EQ(9.f, vec.At(1));
    EXPECT_FLOAT_EQ(1.f, vec.At(2));

    vec.At(0) = 7.f;
    vec.At(1) = 6.f;
    vec.At(3) = 5.f;
    EXPECT_FLOAT_EQ(7.f, vec.x);
    EXPECT_FLOAT_EQ(6.f, vec.y);
    EXPECT_FLOAT_EQ(5.f, vec.w);
}

