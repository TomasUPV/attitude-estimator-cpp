#include "attitude_estimator/quaternion_math.h"
#include <cmath>
#include <limits>

namespace attitude {

float64_t QuaternionMath::norm(const Quaternion& q) noexcept {
    return std::sqrt((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
}

Quaternion QuaternionMath::normalize(const Quaternion& q) noexcept {
    const float64_t n = norm(q);
    constexpr float64_t kEpsilon = 1e-12;

    // Comparación segura frente a subnormales o división por cero (MISRA C++)
    if (n < kEpsilon) {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
    return Quaternion{q.w / n, q.x / n, q.y / n, q.z / n};
}

} // namespace attitude 
