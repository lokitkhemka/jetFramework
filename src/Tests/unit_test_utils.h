#pragma once

#include<Vector/vector3.h>
#include<gtest/gtest.h>

#define EXPECT_VECTOR2_EQ(expected, actual) \
    EXPECT_DOUBLE_EQ(expected.x, actual.x); \
    EXPECT_DOUBLE_EQ(expected.y, actual.y); \

#define EXPECT_VECTOR2_NEAR(expected, actual, eps) \
    EXPECT_NEAR(expected.x, actual.x, eps); \
    EXPECT_NEAR(expected.y, actual.y, eps); \

#define EXPECT_VECTOR3_EQ(expected, actual) \
    EXPECT_DOUBLE_EQ(expected.x, actual.x); \
    EXPECT_DOUBLE_EQ(expected.y, actual.y); \
    EXPECT_DOUBLE_EQ(expected.z, actual.z); \

#define EXPECT_VECTOR3_NEAR(expected, actual, eps) \
    EXPECT_NEAR(expected.x, actual.x, eps); \
    EXPECT_NEAR(expected.y, actual.y, eps); \
    EXPECT_NEAR(expected.z, actual.z, eps); \

#define EXPECT_VECTOR4_EQ(expected, actual) \
    EXPECT_DOUBLE_EQ(expected.x, actual.x); \
    EXPECT_DOUBLE_EQ(expected.y, actual.y); \
    EXPECT_DOUBLE_EQ(expected.z, actual.z); \
    EXPECT_DOUBLE_EQ(expected.w, actual.w); \

#define EXPECT_VECTOR4_NEAR(expected, actual, eps) \
    EXPECT_NEAR(expected.x, actual.x, eps); \
    EXPECT_NEAR(expected.y, actual.y, eps); \
    EXPECT_NEAR(expected.z, actual.z, eps); \
    EXPECT_NEAR(expected.w, actual.w, eps); \