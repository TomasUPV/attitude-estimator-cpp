#include "attitude_estimator/quaternion_math.h"
#include <cmath>

namespace attitude {

double QuaternionMath::norm(const Quaternion& q) {
    return std::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

Quaternion QuaternionMath::normalize(const Quaternion& q) {
    const double n = norm(q);
    if (n <= 0.0) {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
    return Quaternion{q.w / n, q.x / n, q.y / n, q.z / n};
}

} // namespace attitude
