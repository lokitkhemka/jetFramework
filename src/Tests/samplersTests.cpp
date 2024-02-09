#include <constants.h>
#include <Samplers/Samplers.h>
#include <random>
#include <gtest/gtest.h>

using namespace jet;

TEST(Samplers, UniformSampleCone) {
    std::mt19937 mtRand(0);
    std::uniform_real_distribution<double> dist(0, 1);

    for (int i = 0; i < 100; ++i) {
        double u1 = dist(mtRand);
        double u2 = dist(mtRand);

        Vector3D pt = UniformSampleCone(u1, u2, Vector3D(1, 0, 0), 0.5);

        double dot = pt.Dot(Vector3D(1, 0, 0));
        EXPECT_LE(std::cos(0.5), dot);

        double dist = pt.Length();
        EXPECT_DOUBLE_EQ(1.0, dist);
    }
}

TEST(Samplers, UniformSampleHemisphere) {
    std::mt19937 mtRand(0);
    std::uniform_real_distribution<double> dist(0, 1);

    for (int i = 0; i < 100; ++i) {
        double u1 = dist(mtRand);
        double u2 = dist(mtRand);

        Vector3D pt = UniformSampleHemisphere(u1, u2, Vector3D(1, 0, 0));

        double dot = pt.Dot(Vector3D(1, 0, 0));
        EXPECT_LE(std::cos(kHalfPiD), dot);

        double dist = pt.Length();
        EXPECT_DOUBLE_EQ(1.0, dist);
    }
}

TEST(Samplers, UniformSampleSphere) {
    std::mt19937 mtRand(0);
    std::uniform_real_distribution<double> dist(0, 1);

    for (int i = 0; i < 100; ++i) {
        double u1 = dist(mtRand);
        double u2 = dist(mtRand);

        Vector3D pt = UniformSampleSphere(u1, u2);

        double dist = pt.Length();
        EXPECT_DOUBLE_EQ(1.0, dist);
    }
}

TEST(Samplers, UniformSampleDisk) {
    std::mt19937 mtRand(0);
    std::uniform_real_distribution<double> dist(0, 1);

    for (int i = 0; i < 100; ++i) {
        double u1 = dist(mtRand);
        double u2 = dist(mtRand);

        Vector2D pt = UniformSampleDisk(u1, u2);

        double dist = pt.Length();
        EXPECT_GE(1.0, dist);
    }
}