# Design — AttitudeEstimator
Traces to: HLR-001, HLR-002, HLR-003, HLR-004, HLR-007, HLR-008

High-level wrapper module acting as the public-facing interface (Facade pattern) for external consumers. Encapsulates sensor input validation, time-delta tracking, state orchestration, and conversion to human-readable angles.

## Responsibilities
- Coordinates interactions between `SensorModel`, `EKF_Core`, and `QuaternionMath`.
- Manages sensor sample ingress and passes delta-time explicitly to the filter.
- Provides sanitized orientation output in both quaternion and Euler angles.

## Low-Level Requirements
| ID | Requirement |
|---|---|
| LLR-AE-001 | `initialize(initial_orientation)` shall configure the internal `EKF_Core` instance with default covariance matrices and initial state. |
| LLR-AE-002 | `processSample(imu_sample)` shall validate reading integrity (finite values, timestamp monotonicity) before executing the estimation step. |
| LLR-AE-003 | `processSample()` shall compute elapsed time $dt = t_k - t_{k-1}$ between consecutive samples and forward it to `EKF_Core::predict()`. |
| LLR-AE-004 | `processSample()` shall invoke `EKF_Core::update()` using the accelerometer reading from `imu_sample`. |
| LLR-AE-005 | `getOrientationQuaternion()` shall return the current estimated unit quaternion. |
| LLR-AE-006 | `getEulerAngles()` shall compute and return pitch and roll in radians via `QuaternionMath::toEulerAngles()`. |
| LLR-AE-007 | `processSample()` shall discard samples with non-positive or excessive time deltas ($dt \le 0$ or $dt > 0.1\text{ s}$) and report an input error flag. |

## Notes
- **Decoupling**: Consumers do not interact with matrix operations, covariance propagation, or raw Jacobians; everything passes through `processSample()`.
- **Fault Detection (LLR-AE-007)**: Guards against integration explosions if the sensor driver stalls or drops packets.
