# Software Design Standard (SDS)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.7  
**Target Design Assurance Level:** DAL B  
**Baseline Version:** 0.1 (Draft)  
**Status:** In Work / Draft  

---

## 1. Scope & Objective

This Software Design Standard (SDS) defines mandatory architectural principles, design constraints, and interface specifications for the Attitude Estimator software component.

In accordance with RTCA DO-178C Section 5.2 and Section 11.7, this standard guarantees that all module architectures and Low-Level Requirements (LLRs) are:
* Deterministic, modular, and decoupled.
* Strictly bounded in computational and memory execution.
* Directly traceable to parent High-Level Requirements (HLRs).

---

## 2. Architectural Principles & Partitioning

### 2.1 Layered Modular Architecture
The software architecture enforces unidirectional dependencies (Facade pattern):

```text
  [ AttitudeEstimator ]  (Public Facade / Validation / State Orchestrator)
           │
           ├──► [ SensorModel ]      (Inertial Data Structs & Calibration)
           ├──► [ EKF_Core ]        (Covariance, Kalman Gain, Invariants)
           └──► [ QuaternionMath ]  (Stateless Kinematics & Linear Algebra)
```
* **AttitudeEstimator:** Outermost entry boundary. Validates inputs, handles delta-time monotonicity, and coordinates filter invocations.
* **EKF_Core:** Core algorithm encapsulation. Manages state covariance matrices, state propagation, and measurement updates.
* **SensorModel:** Immutable data contracts and synthetic simulation structures.
* **QuaternionMath:** Pure utility library with zero persistent internal state.

### 2.2 Coupling and Cohesion Constraints
* **High Cohesion:** Each module shall fulfill a single dedicated responsibility.
* **Loose Coupling:** Modules communicate exclusively via public member functions and parameter passing. Direct public access to internal state variables is prohibited.
* **Cyclic Dependencies:** Circular dependencies between modules or header includes are strictly forbidden.

---

## 3. Computational and Memory Design Constraints

### 3.1 Deterministic Memory Footprint (Zero Dynamic Heap)
* Dynamic runtime memory allocation (`malloc`, `calloc`, `realloc`, `free`, `new`, `delete`) is prohibited across all functional flight paths.
* All arrays and matrices shall use fixed-capacity containers (`std::array`) with sizes known at compile time.
* Total worst-case stack depth must be statically predictable.

### 3.2 Floating-Point Arithmetic Rules
* Computations shall standardize on IEEE-754 double-precision floating-point types (`attitude::float64_t`).
* All divisions must incorporate defensive guards ($|denominator| \ge \epsilon$) with $\epsilon = 1 \times 10^{-12}$ to eliminate division by zero and subnormal float operations.
* Matrix and vector dimensions must be statically validated to preclude out-of-bounds memory indexing.

### 3.3 Complexity and Control Flow Rules
* **Recursion:** Direct and indirect recursion is strictly forbidden to guarantee bounded execution time (WCET).
* **Single Entry / Predictable Exit:** Functions shall maintain bounded cyclomatic complexity ($V(G) \le 10$) and deterministic exit paths.
## 4. Low-Level Requirements (LLR) Formulation Rules

Every LLR authored in `docs/design/*.md` shall comply with the following structural rules:
1. **Scope of Intent:** An LLR specifies *how* a specific module implements an algorithm, manages an interface, or enforces an invariant.
2. **Interface Contract:** The LLR must define all input parameters, return types, pre-conditions, and post-conditions.
3. **Traceability:** Every LLR must trace backward to at least one parent HLR (`HLR-xxx`) in the Requirements Traceability Matrix.
4. **Modality:** Normative statements shall exclusively utilize the keyword `shall`.

---

## 5. Defensive Design & Fault Handling Patterns

* **Input Data Validation:** Public interfaces must validate sample timestamps for strictly positive monotonicity ($0 < dt \le 0.1\text{ s}$).
* **Normalization Invariant:** Following any state prediction or measurement update, the attitude quaternion must undergo Euclidean normalization.
* **Covariance Symmetry:** The error covariance matrix $P$ must enforce mathematical symmetry (`P = 0.5 * (P + P^T)`) to prevent numerical divergence caused by finite-precision drift.

---

## 6. Software Design Review Checklist (Peer Inspection & SQA Gate)

Prior to baselining software architectural documents (`docs/design/*.md`), the independent reviewer and SQA auditor shall execute and sign this verification checklist:

| Item # | Design Verification Item | DO-178C Criteria Reference | Verification Method | Result (Pass / Fail / N/A) | Evidence / Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-SDS-01** | **Deterministic Memory:** Are all runtime dynamic heap allocations (`malloc`/`new`) excluded in favor of fixed-capacity containers (`std::array`)? | Section 5.2.2.a | Design Inspection | | |
| **CHK-SDS-02** | **Absence of Recursion:** Is the design completely free of recursive algorithms, ensuring bounded WCET? | Section 5.2.2.b | Control Flow Analysis | | |
| **CHK-SDS-03** | **Defensive Division Guards:** Do all division operations specify non-zero threshold guards ($\epsilon \ge 1\times 10^{-12}$)? | Section 5.2.2.b | Interface Inspection | | |
| **CHK-SDS-04** | **Decoupled Architecture:** Does the design enforce strict separation of concerns without cyclic module dependencies? | Section 5.2.1 | Architectural Review | | |
| **CHK-SDS-05** | **LLR Traceability:** Does every LLR trace backward to a parent HLR in `docs/requirements/traceability_matrix.md`? | Section 5.5 / Table A-4 | Traceability Audit | | |
| **CHK-SDS-06** | **State Invariant Protection:** Are quaternion normalization and covariance symmetry invariants documented at module boundaries? | Section 5.2.2.c | Invariant Analysis | | |

### Checklist Sign-off Record
* **Review Target Artifact:** `docs/design/*.md`
* **Artifact Baseline Version:** _______________
* **Author / Submitter:** ______________________  | Date: ____________
* **Independent Peer Reviewer:** ______________  | Date: ____________
* **SQA Gatekeeper Approval:** ________________  | Date: ____________
