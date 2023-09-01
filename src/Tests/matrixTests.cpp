#include<Matrix/matrix.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Matrix, Constructors)
{
    Matrix<double, 2, 3> mat;

    EXPECT_EQ(6u, mat.elements.size());

    for (double elem : mat.elements) {
        EXPECT_DOUBLE_EQ(0.0, elem);
    }
}