# Design — QuaternionMath

Traces to: HLR-001, HLR-005

Pure mathematical operations on quaternions. Stateless — no internal
state, no side effects. Fully unit-testable in isolation.

## Representation

Quaternion stored as 4 doubles: `(w, x, y, z)`, where `w` is the
scalar part. Convention: unit quaternion, body-to-world rotation
(Hamilton convention, right-handed).

## Low-Level Requirements

| ID        | Requirement |
|-----------|-------------|
| LLR-QM-001 | `multiply(q1, q2)` shall return the Hamilton product of two quaternions, representing the composition of two rotations. |
| LLR-QM-002 | `normalize(q)` shall return a quaternion with unit norm, preserving its direction. |
| LLR-QM-003 | `norm(q)` shall return the Euclidean norm of the quaternion's 4 components. |
| LLR-QM-004 | `toEulerAngles(q)` shall return (roll, pitch, yaw) in radians, for debug/visualization purposes only — never used as internal state. |
| LLR-QM-005 | `derivative(q, angular_velocity)` shall return dq/dt given the current quaternion and a 3-axis angular rate vector, per the standard quaternion kinematic equation. |
| LLR-QM-006 | All functions shall be pure (no side effects, no mutation of input arguments). |

## Notes

- `toEulerAngles` exists ONLY for humans reading debug output / plots.
  It must never feed back into the filter state — that's exactly the
  gimbal-lock trap this project is designed to avoid (see scope.md).
- `derivative()` implements: dq/dt = 0.5 * q ⊗ (0, ω_x, ω_y, ω_z),
  where ⊗ is the Hamilton product (LLR-QM-001) and ω is angular
  velocity in rad/s. This is what `EKF_Core::predict()` will
  integrate forward in time.
