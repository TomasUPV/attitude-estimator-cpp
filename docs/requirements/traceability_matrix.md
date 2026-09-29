# Requirements Traceability Matrix (RTM) — Attitude Estimator

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Function  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 5.5, Table A-2 (Obj 6) & Table A-4 (Obj 1)  
**Document Version:** 1.1 (SOI-2 Baseline Candidate)  
**Status:** Synchronized with HLR v1.1  

---

## 1. High-Level Requirements to Low-Level Requirements (Forward Trace)

| HLR ID | HLR Category / Summary | Target LLR ID(s) | Implementing Module / File | Verification Method | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **HLR-FNC-001** | 3D Spatial Orientation via Hamilton Quaternion | `LLR-QM-001`, `LLR-QM-002`, `LLR-QM-003`, `LLR-AE-001`, `LLR-AE-005` | `QuaternionMath`, `AttitudeEstimator` | Test (HLT) | In Progress |
| **HLR-FNC-002** | Discrete Gyro Angular Rate Integration | `LLR-QM-005`, `LLR-EKF-002`, `LLR-EKF-003` | `QuaternionMath`, `EKF_Core` | Test (HLT) | In Progress |
| **HLR-FNC-003** | Accelerometer Gravity Reference Correction | `LLR-EKF-004`, `LLR-EKF-005`, `LLR-EKF-006` | `EKF_Core` | Test (HLT) | In Progress |
| **HLR-FNC-004** | Egress Euler Angle Conversion (Tait-Bryan Z-Y-X) | `LLR-QM-004`, `LLR-AE-006` | `QuaternionMath`, `AttitudeEstimator` | Test (HLT) / Inspection | In Progress |
| **HLR-PRF-001** | Static Error < 1.5° RMS (10.0 s Window) | `LLR-EKF-004`, `LLR-AE-004` | `EKF_Core`, `AttitudeEstimator` | Test (HLT) | In Progress |
| **HLR-PRF-002** | Dynamic Convergence from 30° in < 3.0 s | `LLR-EKF-004`, `LLR-AE-004` | `EKF_Core`, `AttitudeEstimator` | Test (HLT) | In Progress |
| **HLR-PRF-003** | Dynamic Error < 3.0° RMS & < 5.0° Peak (10 min) | `LLR-EKF-002`, `LLR-EKF-003`, `LLR-EKF-006` | `EKF_Core` | Test (HLT) | In Progress |
| **HLR-PRF-004** | Verification via 6-DOF Synthetic Trajectory | `LLR-SM-004`, `LLR-SM-005`, `LLR-SM-006` | `SensorModel` | Test (HLT) | In Progress |
| **HLR-IFC-001** | IMU Ingress Bounds & NaN/Inf Sanitization | `LLR-SM-001`, `LLR-SM-002`, `LLR-SM-003`, `LLR-AE-002` | `SensorModel`, `AttitudeEstimator` | Test (HLT / Robustness) | In Progress |
| **HLR-IFC-002** | Non-blocking Egress Query Facade | `LLR-AE-005`, `LLR-AE-006` | `AttitudeEstimator` | Review / Test (HLT) | In Progress |
| **HLR-SAF-001** | Timestep Bounds (0 < dt <= 0.1s) & Sample Rejection | `LLR-AE-002`, `LLR-AE-003`, `LLR-AE-007` | `AttitudeEstimator` | Test (HLT / Robustness) | In Progress |
| **HLR-SAF-002** | Continuous Unit Quaternion Bound (norm(q) = 1.0) | `LLR-QM-002`, `LLR-EKF-001`, `LLR-EKF-007` | `QuaternionMath`, `EKF_Core` | Test (HLT / Robustness) | In Progress |
| **HLR-SAF-003** | Accel Plausibility Window (7.848 to 11.772 m/s^2) | `LLR-AE-002`, `LLR-EKF-004` | `AttitudeEstimator`, `EKF_Core` | Test (HLT / Robustness) | In Progress |
| **HLR-SAF-004** | Deterministic Execution & Zero Heap Allocation | `LLR-QM-006`, `LLR-EKF-009` | `QuaternionMath`, `EKF_Core` | Static Analysis / Inspection | In Progress |

---

## 2. Low-Level Requirements to High-Level Requirements (Backward Trace)

| LLR ID | Implementing Module | Allocation / Responsibility | Parent HLR ID(s) | Status |
| :--- | :--- | :--- | :--- | :--- |
| **LLR-QM-001** | `QuaternionMath` | Hamilton Product Algorithm | `HLR-FNC-001` | In Progress |
| **LLR-QM-002** | `QuaternionMath` | Unit Normalization with Epsilon Protection | `HLR-FNC-001`, `HLR-SAF-002` | In Progress |
| **LLR-QM-003** | `QuaternionMath` | Euclidean Vector Norm Computation | `HLR-FNC-001` | In Progress |
| **LLR-QM-004** | `QuaternionMath` | Tait-Bryan Z-Y-X Euler Transformation | `HLR-FNC-004` | In Progress |
| **LLR-QM-005** | `QuaternionMath` | Kinematic Derivative Formulation | `HLR-FNC-002` | In Progress |
| **LLR-QM-006** | `QuaternionMath` | Pure Stateless Functions (Zero Heap) | `HLR-SAF-004` | In Progress |
| **LLR-SM-001** | `SensorModel` | `AccelReading` Struct Ingress Definition | `HLR-IFC-001` | In Progress |
| **LLR-SM-002** | `SensorModel` | `GyroReading` Struct Ingress Definition | `HLR-IFC-001` | In Progress |
| **LLR-SM-003** | `SensorModel` | Synchronized `ImuSample` Aggregation | `HLR-IFC-001` | In Progress |
| **LLR-SM-004** | `SensorModel` | 6-DOF Ground-Truth Kinematic Profile Generator | `HLR-PRF-004` | In Progress |
| **LLR-SM-005** | `SensorModel` | Additive Gaussian Noise Simulation | `HLR-PRF-004` | In Progress |
| **LLR-SM-006** | `SensorModel` | Ground-Truth Orientation Output Pairing | `HLR-PRF-004` | In Progress |
| **LLR-EKF-001** | `EKF_Core` | Filter State and Covariance Initialization | `HLR-SAF-002` | In Progress |
| **LLR-EKF-002** | `EKF_Core` | Quaternion Kinematics State Propagation | `HLR-FNC-002`, `HLR-PRF-003` | In Progress |
| **LLR-EKF-003** | `EKF_Core` | Covariance Propagation via Jacobian F | `HLR-FNC-002`, `HLR-PRF-003` | In Progress |
| **LLR-EKF-004** | `EKF_Core` | Accel Measurement Innovation & Plausibility Gate | `HLR-FNC-003`, `HLR-PRF-001`, `HLR-PRF-002`, `HLR-SAF-003` | In Progress |
| **LLR-EKF-005** | `EKF_Core` | Measurement Jacobian H and Kalman Gain | `HLR-FNC-003` | In Progress |
| **LLR-EKF-006** | `EKF_Core` | State Correction and Covariance Symmetrization | `HLR-FNC-003`, `HLR-PRF-003` | In Progress |
| **LLR-EKF-007** | `EKF_Core` | State Quaternion Post-Step Normalization | `HLR-SAF-002` | In Progress |
| **LLR-EKF-008** | `EKF_Core` | Const State Accessor Method | `HLR-FNC-001` | In Progress |
| **LLR-EKF-009** | `EKF_Core` | Compile-Time Static Memory Allocation | `HLR-SAF-004` | In Progress |
| **LLR-AE-001** | `AttitudeEstimator` | Facade Lifecycle Orchestration | `HLR-FNC-001` | In Progress |
| **LLR-AE-002** | `AttitudeEstimator` | Sample Integrity & Monotonicity Validation | `HLR-IFC-001`, `HLR-SAF-001`, `HLR-SAF-003` | In Progress |
| **LLR-AE-003** | `AttitudeEstimator` | Timestep Calculation & Predict Dispatch | `HLR-SAF-001` | In Progress |
| **LLR-AE-004** | `AttitudeEstimator` | Accel Update Dispatch | `HLR-PRF-001`, `HLR-PRF-002` | In Progress |
| **LLR-AE-005** | `AttitudeEstimator` | Estimated Quaternion Query Interface | `HLR-FNC-001`, `HLR-IFC-002` | In Progress |
| **LLR-AE-006** | `AttitudeEstimator` | Euler Angles Monitoring Query Interface | `HLR-FNC-004`, `HLR-IFC-002` | In Progress |
| **LLR-AE-007** | `AttitudeEstimator` | Fault Latching & Timestep Rejection | `HLR-SAF-001` | In Progress |
