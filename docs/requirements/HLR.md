# High-Level Requirements — Attitude Estimator

| ID       | Requirement | Rationale / Source |
|----------|-------------|---------------------|
| HLR-001  | The system shall estimate the 3D orientation of a rigid body as a unit quaternion. | scope.md — Filter approach |
| HLR-002  | The system shall accept 3-axis accelerometer and 3-axis gyroscope measurements as input, sampled at 100 Hz. | scope.md — Sensors modeled |
| HLR-003  | The system shall propagate the orientation estimate between measurements using gyroscope angular rate integration. | scope.md — Propagation |
| HLR-004  | The system shall correct the orientation estimate using accelerometer measurements as a gravity vector reference. | scope.md — Correction |
| HLR-005  | The estimated quaternion shall remain normalized (unit norm) at all times. | Mathematical validity of quaternion representation |
| HLR-006  | The system shall achieve a steady-state orientation error of less than 1.5° (roll/pitch) under static conditions. | scope.md — Success metrics |
| HLR-007  | The system shall converge from a 30° initial orientation error to within the steady-state error bound in under 3 seconds. | scope.md — Success metrics |
| HLR-008  | The system shall not diverge (unbounded error growth) over a 10-minute continuous operation profile. | scope.md — Success metrics |
| HLR-009  | The system shall be verifiable against synthetically generated ground-truth trajectories with known orientation at each timestep. | scope.md — Test data |
