# Design — EKF_Core
Traces to: HLR-003, HLR-004, HLR-005, HLR-006, HLR-007, HLR-008

State estimation core implementing an Extended Kalman Filter (EKF) for attitude estimation. Maintains system state, covariance, and computes the prediction-update cycle using quaternion kinematics and gravity vector observations.

## State Representation
- State vector $\mathbf{x}$: 4-element unit quaternion $\mathbf{q} = [w, x, y, z]^T$.
- Error covariance matrix $\mathbf{P}$: $4 \times 4$ positive semi-definite matrix representing state uncertainty.
- Process noise covariance $\mathbf{Q}$: $4 \times 4$ matrix tuned to gyroscope noise characteristics.
- Measurement noise covariance $\mathbf{R}$: $3 \times 3$ matrix tuned to accelerometer noise characteristics.

## Low-Level Requirements
| ID | Requirement |
|---|---|
| LLR-EKF-001 | `initialize(q_init, P_init)` shall set the internal state quaternion and covariance matrix, asserting unit norm on `q_init`. |
| LLR-EKF-002 | `predict(gyro_reading, dt)` shall accept explicit timestep `dt` and angular rate vector to propagate state quaternion using quaternion kinematics integration. |
| LLR-EKF-003 | `predict()` shall compute the state transition Jacobian $\mathbf{F}$ and propagate error covariance via $\mathbf{P}_{k|k-1} = \mathbf{F} \mathbf{P}_{k-1|k-1} \mathbf{F}^T + \mathbf{Q}$. |
| LLR-EKF-004 | `update(accel_reading)` shall normalize the accelerometer measurement before computing the innovation vector against the expected gravity direction in body frame. |
| LLR-EKF-005 | `update()` shall compute the measurement Jacobian $\mathbf{H}$ and Kalman Gain $\mathbf{K} = \mathbf{P} \mathbf{H}^T (\mathbf{H} \mathbf{P} \mathbf{H}^T + \mathbf{R})^{-1}$. |
| LLR-EKF-006 | `update()` shall compute state correction $\mathbf{x} = \mathbf{x} + \mathbf{K}\mathbf{y}$ and update error covariance $\mathbf{P} = (\mathbf{I} - \mathbf{K}\mathbf{H}) \mathbf{P}$. |
| LLR-EKF-007 | `normalizeState()` shall normalize the state quaternion after every predict and update step to prevent drift from unit length. |
| LLR-EKF-008 | `getState()` shall return the current attitude quaternion without exposing internal mutable state references. |
| LLR-EKF-009 | The module shall avoid dynamic memory allocations (`new`, `malloc`) after initialization to comply with deterministic real-time execution. |

## Notes
- **Zero-yaw reference**: Without magnetometer data, yaw orientation has no global reference and is subject to unconstrained drift. The observation model relies purely on gravity ($[0, 0, -1g]$) to correct pitch and roll.
- **Covariance symmetry**: Numerical integration can introduce asymmetry in $\mathbf{P}$. The update step should enforce $\mathbf{P} = \frac{1}{2}(\mathbf{P} + \mathbf{P}^T)$ if numerical divergence is detected during testing.
- **Determinism (LLR-EKF-009)**: Matrix dimensions are fixed at compile time (e.g., via `std::array` or fixed-size structs), ensuring $O(1)$ temporal and spatial memory determinism.
