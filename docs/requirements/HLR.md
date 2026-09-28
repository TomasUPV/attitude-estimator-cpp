# High-Level Requirements (HLR) — Attitude Estimator

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Software Function  
**Target Process:** RTCA DO-178C / EUROCAE ED-12C Section 5.1 & Table A-2  
**Governing Standard:** `docs/standards/SRS.md`  
**Target Baseline:** DAL B (with DAL C Standby Applicability)  
**Document Version:** 1.0 (SOI-2 Baseline)  
**Status:** Released for SOI-2 Review

---

## 1. Functional Requirements (HLR-FNC)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-FNC-001** | The system shall estimate the 3D spatial orientation of the rigid body represented as a 4-element Hamilton unit quaternion ($q = [w, x, y, z]^T$, body-to-world frame). | Scope § Filter approach (Eliminates gimbal lock) | Test (HLT) |
| **HLR-FNC-002** | The system shall propagate the attitude state estimate between discrete measurement epochs via numerical integration of gyroscope tri-axial angular rates. | Scope § Propagation; DO-178C Table A-2 (Obj 1) | Test (HLT) |
| **HLR-FNC-003** | The system shall correct the propagated attitude state estimate utilizing tri-axial accelerometer measurements as a local gravity vector reference ($[0, 0, -1g]^T$). | Scope § Correction; DO-178C Table A-2 (Obj 1) | Test (HLT) |
| **HLR-FNC-004** | The system shall provide an auxiliary transformation converting the internal attitude quaternion into Euler angles (pitch, roll, yaw) in radians strictly for telemetry/display egress. | Scope § Filter approach; `docs/standards/SDS.md` | Test (HLT) / Inspection |

---

## 2. Performance & Accuracy Requirements (HLR-PRF)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-PRF-001** | Under nominal static conditions, the system shall maintain a steady-state pitch and roll orientation error of less than $1.5^\circ$ ($0.02618\text{ rad}$) RMS relative to ground truth. | Scope § Success metrics (MEMS noise bound) | Test (HLT) |
| **HLR-PRF-002** | The system shall converge from an initial angular orientation displacement of $30.0^\circ$ to within the steady-state error bound ($\le 1.5^\circ$) in less than $3.0\text{ s}$. | Scope § Success metrics | Test (HLT) |
| **HLR-PRF-003** | The system shall maintain bounded orientation error without divergence across a continuous $10.0\text{-minute}$ ($600.0\text{ s}$) dynamic simulated flight profile. | Scope § Success metrics; Numerical stability | Test (HLT) |
| **HLR-PRF-004** | The system shall be verifiable against synthetically generated ground-truth trajectories pairing deterministic 6-DOF IMU samples with true orientation at each timestep. | Scope § Test data; DO-178C Table A-2 (Obj 2) | Test (HLT) |

---

## 3. Interface & Timing Requirements (HLR-IFC)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-IFC-001** | The system shall ingest synchronized tri-axial specific force ($a_x, a_y, a_z$) in $\text{m/s}^2$ and tri-axial angular rates ($\omega_x, \omega_y, \omega_z$) in $\text{rad/s}$ with a nominal epoch interval of $\Delta t = 0.01\text{ s}$ ($100\text{ Hz}$). | Scope § Sensors modeled; System Allocation | Test (HLT) |
| **HLR-IFC-002** | The system shall provide non-blocking query interfaces returning the current estimated quaternion and Euler angles without modifying internal filter state. | `docs/standards/SDS.md` § Loose Coupling | Review / Test (HLT) |

---

## 4. Safety & Integrity Requirements (HLR-SAF) [Derived Requirements]

*Note: Requirements in this section are derived directly from the system safety assessment (`FHA_summary.md`) to mitigate Hazardously Misleading Information (FHA-AHRS-001).*

| Requirement ID | Statement | Parent Safety Allocation | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-SAF-001** | The system shall reject and flag as invalid any sensor sample whose elapsed timestep satisfies $\Delta t \le 0.0\text{ s}$ or $\Delta t > 0.10\text{ s}$ to prevent kinematic integrator explosion. | `SR-SAF-001` (FHA § 5) | Test (HLT / Robustness) |
| **HLR-SAF-002** | The system shall guarantee that the Euclidean norm of the estimated attitude quaternion remains strictly bounded ($\vert{}\Vert{}q\Vert{} - 1.0\vert{} \le 1.0\times 10^{-6}$) after every prediction and update cycle. | `SR-SAF-002` (FHA § 5) | Test (HLT / Robustness) |
| **HLR-SAF-003** | The system shall reject accelerometer correction updates when the measured acceleration norm deviates from nominal gravity by more than $\pm 20\%$ ($\Vert{}a\Vert{} < 7.848\text{ m/s}^2$ or $\Vert{}a\Vert{} > 11.772\text{ m/s}^2$). | FHA-AHRS-001 (Prevents corrupted gravity vectors under dynamic linear acceleration) | Test (HLT / Robustness) |
| **HLR-SAF-004** | The system shall execute without dynamic runtime heap allocation (`malloc`, `free`, `new`, `delete`), utilizing statically bounded memory allocations. | `SR-SAF-003` (FHA § 5); `docs/standards/SCS.md` | Static Analysis / Inspection |

---

## 5. Requirements Review Checklist (DO-178C Table A-2 Verification)

Prior to baselining these High-Level Requirements, the independent peer reviewer and SQA gatekeeper evaluate compliance against `docs/standards/SRS.md` and DO-178C Table A-2 objectives:

| Item # | Verification Check Item | DO-178C Criteria Reference | Verification Method | Result (Pass / Fail / In Work) | Review Findings / Evidence |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-SRS-01** | **Mandatory Modality:** Does every requirement state normative intent using `shall` syntax, excluding ambiguous verbs (`should`, `may`, `will`)? | Section 5.1.2.a | Visual Inspection | **[X] Pass** | 100% compliance across all 14 HLRs; no ambiguous verbs found. |
| **CHK-SRS-02** | **Absence of Ambiguity:** Is the statement free of qualitative/untestable terms (e.g., *fast*, *robust*, *optimized*, *TBD*)? | Section 5.1.2.b | Lexical Search / Peer Review | **[ ] Fail** | Finding F-HLR-01: `HLR-PRF-003` uses unquantified phrase "without divergence". Finding F-HLR-02: `HLR-FNC-004` omits Euler sequence convention. |
| **CHK-SRS-03** | **Verifiability & Quantifiable Tolerances:** Does the requirement establish numerical thresholds, timing bounds, SI units, and deterministic limits? | Section 6.2.2.a | Review vs Scope Targets | **[ ] Fail** | Finding F-HLR-01: Dynamic error ceiling missing in `HLR-PRF-003`. Finding F-HLR-02: Steady-state evaluation window missing in `HLR-PRF-001`. |
| **CHK-SRS-04** | **Input/Output Domain Completeness:** Are nominal operating intervals, boundary/edge conditions, and invalid inputs explicitly addressed? | Section 5.1.2.b | Boundary Analysis Review | **[ ] Fail** | Finding F-HLR-03: `HLR-IFC-001` lacks sensor saturation/NaN boundaries. Finding F-HLR-04: Fallback output state in `HLR-SAF-001` unstated. |
| **CHK-SRS-05** | **Implementation Independence:** Does the HLR define functional intent without dictating programming language constructs or local variables? | Section 5.1.2.a | Architecture Decoupling Check | **[X] Pass** | Functional intent clearly separated from language implementation; memory allocation constraints in `HLR-SAF-004` reflect valid safety allocations. |
| **CHK-SRS-06** | **Traceability Integrity:** Is the requirement assigned an immutable identifier and registered bidirectionally in the RTM? | Section 5.5 / Table A-2 | Traceability Audit | **[ ] Fail** | Finding F-HLR-05: Critical desynchronization. `traceability_matrix.md` and LLR design documents still reference obsolete `HLR-001`..`HLR-009`. |
| **CHK-SRS-07** | **Derived Safety Identification:** If the requirement is derived (e.g., `HLR-SAF-*`), has it been formally flagged and reported to system safety? | Section 5.2.2 / FHA | Safety Assessment Check | **[X] Pass** | All derived safety requirements (`HLR-SAF-001..004`) are formally flagged and traced to `FHA_summary.md` (`SR-SAF-001..003`). |

### Review & Sign-off Record
* **Target Baseline:** `docs/requirements/HLR.md` (v1.0 Baseline Draft)
* **Author / Submitter:** Software Engineering Team | Date: 2026-09-28
* **Independent Reviewer (Verification Role):** Independent Verification Agent (Simulation) | Date: 2026-09-28
* **SQA Gatekeeper Approval:** SQA Gatekeeper Audit Role (Simulation) — *Conditionally Approved (See `docs/verification/SOI_2_peer_review_report.md`)* | Date: 2026-09-28
