#include "attitude_estimator/attitude_estimator.h"

namespace attitude {

void AttitudeEstimator::reset() {
    state_ = QuaternionMath::normalize(Quaternion{1.0, 0.0, 0.0, 0.0});
}

Quaternion AttitudeEstimator::getOrientation() const {
    return state_;
}

} // namespace attitude
