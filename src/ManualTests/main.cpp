#include "manual_tests.h"

#include <jet.h>

#include <gtest/gtest.h>
#include <fstream>

using namespace jet;


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "SPHSolver2*";
    CreateDir(JET_TESTS_OUTPUT_DIR);

    std::ofstream logFile("./manual_tests_output/manual_tests.log");
    if (logFile) {
        Logging::SetAllStream(&logFile);
    }

    int ret = RUN_ALL_TESTS();

    return ret;
}