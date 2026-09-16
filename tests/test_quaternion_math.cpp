#include <gtest/gtest.h>
#include "attitude_estimator/quaternion_math.h"

TEST(QuaternionMathTest, DefaultQuaternionHasUnitNorm) {
    attitude::Quaternion q;
    double n = attitude::QuaternionMath::norm(q);
    EXPECT_NEAR(n, 1.0, 1e-9);
}

TEST(QuaternionMathTest, NormalizationProducesUnitLength) {
    attitude::Quaternion q{2.0, 2.0, 2.0, 2.0};
    attitude::Quaternion q_norm = attitude::QuaternionMath::normalize(q);
    double n = attitude::QuaternionMath::norm(q_norm);
    EXPECT_NEAR(n, 1.0, 1e-9);
}
