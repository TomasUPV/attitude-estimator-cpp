# Software Architecture Description (SAD) — Attitude Estimator

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Core  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 5.2 & Table A-3  
**Design Standard Reference:** `docs/standards/SDS.md`  
**Target Baseline:** DAL B (with DAL C Standby Applicability)  
**Document Version:** 1.1 (SOI-2 Review Candidate)  
**Status:** Submitted for SOI-2 Re-Audit  

---

## 1. Architectural Overview & Modular Decomposition

The Attitude Estimator architecture is designed around four decoupled, single-responsibility modules operating under a strictly unidirectional control and data-flow paradigm.

![Figure 1.1: Functional Architecture & Data-Control Flow Diagram](../images/architecture_flow.png)

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

struct ImuMeasurement {
    Vector3 accel;         
    Vector3 gyro;          
    float64_t dt{0.01}; 
};

using Matrix4x4 = std::array<float64_t, 16>;
using Matrix3x3 = std::array<float64_t, 9>;

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

---

## 5. Architectural Review Checklist (DO-178C Table A-3 Verification)

| Item # | Verification Criteria | DO-178C Reference | Verification Method | Result (Pass / Fail / In Work) | Review Findings / Evidence |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-ARC-01** | Are software architecture requirements compatible with High-Level Requirements? | Table A-3 (Obj 1) | Analysis / Trace | [ ] | |
| **CHK-ARC-02** | Is the software architecture consistent with the Software Design Standard (`SDS.md`)? | Table A-3 (Obj 2) | Visual Inspection | [ ] | |
| **CHK-ARC-03** | Is the architecture deterministic (no recursion, zero runtime heap allocation)? | Table A-3 (Obj 3) | Visual Inspection | [ ] | |
| **CHK-ARC-04** | Are interfaces and data flow between modules explicitly defined and bounded? | Table A-3 (Obj 4) | Interface Review | [ ] | |
| **CHK-ARC-05** | Are partition boundaries and safety-derived invariants enforced against faults? | Table A-3 (Obj 5) | Boundary Review | [ ] | |

### Review & Sign-off Record
* **Target Baseline:** `docs/design/architecture.md` (v1.1 Candidate)
* **Author / Submitter:** Software Development Team | Date: 2026-09-28
* **Independent Reviewer (Verification Role):** ____________________ | Date: ____________
* **SQA Gatekeeper Approval:** ____________________ | Date: ____________
