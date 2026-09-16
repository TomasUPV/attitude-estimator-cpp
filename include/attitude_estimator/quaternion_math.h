#pragma once

namespace attitude {

struct Quaternion {
    double w{1.0};
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

class QuaternionMath {
public:
    static double norm(const Quaternion& q);
    static Quaternion normalize(const Quaternion& q);
};

} // namespace attitude
