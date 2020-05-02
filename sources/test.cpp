#include <gtest/gtest.h>
#include "determinant.h"

TEST(determinant, random_10x10_432) {
    auto matrix = Matrix<double>::random(10, 10, 423);
    ASSERT_NEAR(472.7689765, determinant(matrix, false), 1e-7);
    ASSERT_NEAR(472.7689765, determinant(matrix, true), 1e-7);
}

TEST(determinant, random_15x15_432) {
    auto matrix = Matrix<double>::random(15, 15, 423);
    ASSERT_NEAR(-276749.7284771, determinant(matrix, false), 1e-7);
    ASSERT_NEAR(-276749.7284771, determinant(matrix, true), 1e-7);
}

TEST(determinant, random_128x128_31) {
    std::normal_distribution<double> distribution{0, 0.5};
    auto matrix = Matrix<double>::random(15, 15, 31, distribution);
    ASSERT_NEAR(1.3561929, determinant(matrix, false), 1e-7);
    ASSERT_NEAR(1.3561929, determinant(matrix, true), 1e-7);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}