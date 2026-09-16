# Attitude Estimator — DO-178C-Inspired Process

An Extended Kalman Filter (EKF) attitude estimator for 6-DOF IMU data (accelerometer + gyroscope), developed adhering to the safety-critical software lifecycle principles of **DO-178C** and a **MISRA-like** coding standard.

---

## 1. Problem & Motivation
Consumer and aerospace IMU systems require drift-free pitch and roll estimation under noisy dynamics. In safety-critical avionics, algorithmic accuracy is insufficient on its own: code must be deterministic, trace directly to verified requirements, avoid dynamic memory allocation at runtime, and maintain high test coverage.

## 2. Architecture & Design Principles
- **Hamilton Quaternion Kinematics**: Complete avoidance of gimbal lock; Euler angles computed solely for telemetry/inspection.
- **DO-178C-Inspired Traceability**: Strict hierarchy linking Scope $\rightarrow$ High-Level Requirements (HLR) $\rightarrow$ Low-Level Requirements (LLR) $\rightarrow$ Verification Tests.
- **Deterministic C++17**: No runtime dynamic memory allocations (`malloc`, `new`), strict MISRA-aligned static analysis via Clang-Tidy and Cppcheck.
- **Modular Layering**:
  - `QuaternionMath`: Pure mathematical operations on rotations.
  - `SensorModel`: IMU data structures and synthetic noise generators.
  - `EKF_Core`: Numerical integration, Jacobian calculation, and state/covariance updates.
  - `AttitudeEstimator`: Consumer facade handling delta-time computation and input validation.

## 3. Toolchain & Verification Setup
- **Build System**: CMake 3.16+ with `-Wall -Wextra -Wpedantic -Werror -Wconversion`.
- **Unit Testing**: GoogleTest integrated via CMake `FetchContent`.
- **Static Analysis**: Dual-layer linting with `cppcheck` and `clang-tidy` (enforcing CERT C++ and Core Guidelines).
- **CI/CD**: Automated GitHub Actions workflow executing build, tests, and static checks on every push.

### Building & Running Tests Locally
```bash
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
