# Software Architecture Description (SAD) — Attitude Estimator

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Core  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 5.2 & Table A-3  
**Design Standard Reference:** `docs/standards/SDS.md`  
**Target Baseline:** DAL B (with DAL C Standby Applicability)  
**Document Version:** 1.1 (SOI-2 Approved)  
**Status:** SOI-2 Approved  

---

## 1. Architectural Overview & Modular Decomposition

The Attitude Estimator architecture is designed around four decoupled, single-responsibility modules operating under a strictly unidirectional control and data-flow paradigm.

![Figure 1.1: Functional Architecture](docs/images/architecture.svg)

## 2. Core Modules & Responsibilities

| Module | Design File | Responsibility & Coupling Bounds |
| :--- | :--- | :--- |
| **`QuaternionMath`** | `docs/design/quaternion_math.md` | Pure algorithmic operations, Hamilton product, vector rotation, and normalization. Stateless and decoupled. |
| **`SensorModel`** | `docs/design/sensor_model.md` | Ingress data types (`ImuSample`) and synthetic 6-DOF sensor trajectory harness for verification. |
| **`EKF_Core`** | `docs/design/ekf_core.md` | Algorithmic filter mechanics: covariance integration, Kalman gain, innovation, and symmetry enforcement ($P = \frac{1}{2}(P + P^T)$). |
| **`AttitudeEstimator`** | `docs/design/attitude_estimator.md` | Public API facade. Orchestrates timestamp checks, validates $0 < \Delta t \le 0.10\text{ s}$, and encapsulates state. |

---

## 3. Data Representation & Static Type Definitions

In strict conformance with `docs/standards/SCS.md`, the architecture specifies static, fixed-width types in `namespace attitude` and forbids runtime heap allocation:

```cpp
namespace attitude {

using float64_t = double;

struct Vector3 {
    float64_t x{0.0};
    float64_t y{0.0};
    float64_t z{0.0};
};

struct Quaternion {
    float64_t w{1.0};
    float64_t x{0.0};
    float64_t y{0.0};
    float64_t z{0.0};
};

struct EulerAngles {
    float64_t roll{0.0};
    float64_t pitch{0.0};
    float64_t yaw{0.0};    
};

// Sensor reading data structures (harmonized with docs/design/sensor_model.md)
struct AccelReading {
    Vector3 accel;
    float64_t timestamp{0.0};
};

struct GyroReading {
    Vector3 gyro;
    float64_t timestamp{0.0};
};

struct ImuSample {
    AccelReading accel;
    GyroReading gyro;
    float64_t dt{0.01};
};

// Backward-compatibility alias for ingress measurements
using ImuMeasurement = ImuSample;

// Matrix storage convention: Row-Major layout order.
// Element (i, j) of an M x N matrix maps to 1D flat array index: k = i * N + j.
using Matrix4x4 = std::array<float64_t, 16>; // 4x4 Row-Major matrix
using Matrix3x3 = std::array<float64_t, 9>;  // 3x3 Row-Major matrix

enum class FilterStatus : std::uint8_t {
    STATUS_OK = 0U,
    STATUS_INVALID_TIMESTEP = 1U,
    STATUS_DEGENERATE_ACCEL = 2U,
    STATUS_STALE_DATA = 3U
};

}
```
## 4. Deterministic Invariants & Safety Mitigations

In accordance with DO-178C Table A-3 Objectives and `FHA_summary.md`:

1. **Deterministic Execution & Memory Limits:**
   * Dynamic heap allocation (`new`, `malloc`, `calloc`, `free`, `delete`) is completely excluded across all execution paths.
   * Recursion (direct or indirect) is forbidden to guarantee bounded Worst-Case Execution Time (WCET).
   * Maximum function stack consumption is bounded at design time.

2. **Computational Guardrails & Division by Zero:**
   * Every division operation (e.g., vector or quaternion normalization) is protected by an explicit defensive epsilon threshold ($\epsilon = 1.0\times 10^{-12}$). If norm $\le \epsilon$, a safe default unit orientation is enforced.

3. **Temporal Monotonicity & Step Guardrails:**
   * The facade strictly checks $0.0 < \Delta t \le 0.10\text{ s}$. Any non-compliant sample is discarded, maintaining the last verified attitude state and setting an input error flag.

4. **Sensor Plausibility Gate:**
   * Accelerometer correction updates are rejected when $\Vert{}a\Vert{}$ falls outside $[7.848, 11.772]\text{ m/s}^2$ ($\pm 20\%$ nominal gravity), falling back to pure gyroscopic kinematic propagation.

5. **Covariance Matrix Symmetry & Positive Semidefiniteness:**
   * Numerical truncation in floating-point operations can induce asymmetry in the error covariance matrix $P$. The filter architecture enforces $P = \frac{1}{2}(P + P^T)$ after every update step.

6. **Stale Data Fault Containment (`FHA-AHRS-001` Mitigation):**
   * The estimator maintains an invalid-sample counter $N_{drop}$. If consecutive dropped samples satisfy $N_{drop} > 10$ ($> 100\text{ ms}$ at $100\text{ Hz}$), the filter shall assert `FilterStatus::STATUS_STALE_DATA` to prevent silent delivery of frozen telemetry.

---

## 5. Architectural Review Checklist (DO-178C Table A-3 Verification)

| Item # | Verification Criteria | DO-178C Reference | Verification Method | Result (Pass / Fail / In Work) | Review Findings / Evidence |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-ARC-01** | Are software architecture requirements compatible with High-Level Requirements? | Table A-3 (Obj 1) | Analysis / Trace | **[X] Pass** | Finding F-ARC-01 closed: Invariant 4 allocates HLR-SAF-003 ([7.848, 11.772] m/s² plausibility window). |
| **CHK-ARC-02** | Is the software architecture consistent with the Software Design Standard (`SDS.md`)? | Table A-3 (Obj 2) | Visual Inspection | **[X] Pass** | Finding F-ARC-02 closed: Uses `namespace attitude`, `float64_t`, and harmonized data contracts (`ImuSample`, `AccelReading`, `GyroReading`). |
| **CHK-ARC-03** | Is the architecture deterministic (no recursion, zero runtime heap allocation)? | Table A-3 (Obj 3) | Visual Inspection | **[X] Pass** | Invariant 1 verified: Zero runtime heap, no recursion, compile-time static `std::array`, bounded stack depth. |
| **CHK-ARC-04** | Are interfaces and data flow between modules explicitly defined and bounded? | Table A-3 (Obj 4) | Interface Review | **[X] Pass** | Finding F-ARC-03 closed: Explicit Row-Major layout specification with indexing rule (k = i * N + j) for `Matrix4x4` and `Matrix3x3`. |
| **CHK-ARC-05** | Are partition boundaries and safety-derived invariants enforced against faults? | Table A-3 (Obj 5) | Boundary Review | **[X] Pass** | Finding F-ARC-04 closed: Invariant 6 enforces stale-data mitigation (N_drop > 10 asserts STATUS_STALE_DATA) against Hazard FHA-AHRS-001 (HMI). |

### Review & Sign-off Record
* **Target Baseline:** `docs/design/architecture.md` (v1.1 SOI-2 Approved)
* **Author / Submitter:** Software Development Team | Date: 2026-09-29
* **Independent Reviewer (Verification Role):** AI Verification Simulation Auditor | Date: 2026-09-29
* **SQA Gatekeeper Approval:** AI SQA Simulation Auditor — **Approved (Lock v0.2.0-SOI-2)** | Date: 2026-09-29
