#ifndef ATTITUDE_ESTIMATOR_QUATERNION_MATH_H
#define ATTITUDE_ESTIMATOR_QUATERNION_MATH_H

#include "attitude_estimator/types.h"

namespace attitude {

struct Quaternion {
    float64_t w{1.0};
    float64_t x{0.0};
    float64_t y{0.0};
    float64_t z{0.0};
};

class QuaternionMath {
public:
    // Clase de utilidades estáticas: construcción explícitamente deshabilitada
    QuaternionMath() = delete;

    static float64_t norm(const Quaternion& q) noexcept;
    static Quaternion normalize(const Quaternion& q) noexcept;
};

} // namespace attitude

#endif // ATTITUDE_ESTIMATOR_QUATERNION_MATH_H
