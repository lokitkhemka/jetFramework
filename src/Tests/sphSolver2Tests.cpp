#include <ParticleSim/SPH/sph_solver2.h>
#include <gtest/gtest.h>

using namespace jet;

TEST(SPHSolver2, UpdateEmpty) {
    // Empty solver test
    SPHSolver2 solver;
    Frame frame(1, 0.01);
    solver.Update(frame);
    solver.Update(frame);
}

TEST(SPHSolver2, Parameters) {
    SPHSolver2 solver;

    solver.SetEOSExponent(5.0);
    EXPECT_DOUBLE_EQ(5.0, solver.EOSExponent());

    solver.SetEOSExponent(-1.0);
    EXPECT_DOUBLE_EQ(1.0, solver.EOSExponent());

    solver.SetNegativePressureScale(0.3);
    EXPECT_DOUBLE_EQ(0.3, solver.NegativePressureScale());

    solver.SetNegativePressureScale(-1.0);
    EXPECT_DOUBLE_EQ(0.0, solver.NegativePressureScale());

    solver.SetNegativePressureScale(3.0);
    EXPECT_DOUBLE_EQ(1.0, solver.NegativePressureScale());

    solver.SetViscosityCoefficient(0.3);
    EXPECT_DOUBLE_EQ(0.3, solver.ViscosityCoefficient());

    solver.SetViscosityCoefficient(-1.0);
    EXPECT_DOUBLE_EQ(0.0, solver.ViscosityCoefficient());

    solver.SetPseudoViscosityCoefficient(0.3);
    EXPECT_DOUBLE_EQ(0.3, solver.PseudoViscosityCoefficient());

    solver.SetPseudoViscosityCoefficient(-1.0);
    EXPECT_DOUBLE_EQ(0.0, solver.PseudoViscosityCoefficient());

    solver.SetSpeedOfSound(0.3);
    EXPECT_DOUBLE_EQ(0.3, solver.SpeedOfSound());

    solver.SetSpeedOfSound(-1.0);
    EXPECT_GT(solver.SpeedOfSound(), 0.0);

    solver.SetTimeStepLimitScale(0.3);
    EXPECT_DOUBLE_EQ(0.3, solver.TimeStepLimitScale());

    solver.SetTimeStepLimitScale(-1.0);
    EXPECT_DOUBLE_EQ(0.0, solver.TimeStepLimitScale());

    EXPECT_TRUE(solver.SPHSystemData() != nullptr);
}
