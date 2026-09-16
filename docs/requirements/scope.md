# Scope — Attitude Estimator

## Objective
Estimate the 3D orientation (attitude) of a rigid body in real time,
using simulated/logged IMU data, via an Extended Kalman Filter (EKF)
operating on quaternions. Development follows a DO-178C-inspired
process (not certified, no formal DER review) to demonstrate
requirements-driven, traceable, verifiable embedded software practice.

## Sensors modeled
- 3-axis accelerometer (gravity vector reference)
- 3-axis gyroscope (angular rate, integrated for propagation)
- (Stretch goal) 3-axis magnetometer, for yaw observability

## Filter approach
- State: quaternion (4 states) representing body-to-world orientation
- Propagation: gyroscope integration (process model)
- Correction: accelerometer measurement update (gravity reference)
- Rationale: quaternions avoid gimbal lock and are the industry
  standard for aerospace attitude representation, unlike Euler angles

## Success metrics
- Steady-state orientation error < 1.5° (roll/pitch) under static conditions
- Filter converges from a >30° initial orientation error within 3 seconds
- No divergence over a 10 minute simulated flight/motion profile

## Test data
- Synthetic IMU data generated from known ground-truth trajectories
  (so error can be measured exactly)
- (Stretch goal) real logged IMU data if hardware becomes available

## Out of scope (v1)
- Sensor calibration / bias estimation
- Magnetometer fusion (unless stretch goal is reached)
- Real hardware deployment (this is phase 2, separate milestone)
- Formal DO-178C certification artifacts (DER review, tool qualification)

> Note: initial targets based on typical MEMS accelerometer/gyroscope noise characteristics (no magnetometer). To be validated empirically
against simulated data once the filter is implemented; this section will be updated with measured results.
