#pragma once

#include "attitude_estimator/quaternion_math.h"

namespace attitude {

class AttitudeEstimator {
public:
    void reset();
    Quaternion getOrientation() const;

private:
    Quaternion state_{1.0, 0.0, 0.0, 0.0};
};

} // namespace attitude
