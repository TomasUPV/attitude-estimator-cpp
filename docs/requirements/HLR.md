# High-Level Requirements (HLR) — Attitude Estimator

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Software Function  
**Target Process:** RTCA DO-178C / EUROCAE ED-12C Section 5.1 & Table A-2  
**Governing Standard:** `docs/standards/SRS.md`  
**Target Baseline:** DAL B (with DAL C Standby Applicability)  
**Document Version:** 1.1 (SOI-2 Approved)  
**Status:** SOI-2 Approved  

---

## 1. Functional Requirements (HLR-FNC)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-FNC-001** | The system shall estimate the 3D spatial orientation of the rigid body represented as a 4-element Hamilton unit quaternion ($q = [w, x, y, z]^T$, body-to-world frame). | Scope § Filter approach (Eliminates gimbal lock) | Test (HLT) |
| **HLR-FNC-002** | The system shall propagate the attitude state estimate between discrete measurement epochs via numerical integration of gyroscope tri-axial angular rates. | Scope § Propagation; DO-178C Table A-2 (Obj 1) | Test (HLT) |
| **HLR-FNC-003** | The system shall correct the propagated attitude state estimate utilizing tri-axial accelerometer measurements as a local gravity vector reference ($[0, 0, -1g]^T$). | Scope § Correction; DO-178C Table A-2 (Obj 1) | Test (HLT) |
| **HLR-FNC-004** | The system shall provide an auxiliary transformation converting the internal attitude quaternion into Euler angles (roll, pitch, yaw) in radians conforming to intrinsic Tait-Bryan $Z-Y-X$ sequence ($\psi \to \theta \to \phi$), clamping pitch strictly within $[-\pi/2, +\pi/2]$ with deterministic handling when `|sin(θ)| >= 1.0 - 1.0e-6`. | Scope § Filter approach; `docs/standards/SDS.md`; Finding F-HLR-02 | Test (HLT) / Inspection |

---

## 2. Performance & Accuracy Requirements (HLR-PRF)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-PRF-001** | Under nominal static conditions, the system shall maintain a steady-state pitch and roll orientation error of less than $1.5^\circ$ ($0.02618\text{ rad}$) RMS relative to ground truth evaluated across an observation window of at least $10.0\text{ s}$. | Scope § Success metrics (MEMS noise bound); Finding F-HLR-02 | Test (HLT) |
| **HLR-PRF-002** | The system shall converge from an initial angular orientation displacement of $30.0^\circ$ to within the steady-state error bound ($\le 1.5^\circ$) in less than $3.0\text{ s}$. | Scope § Success metrics | Test (HLT) |
| **HLR-PRF-003** | The system shall maintain orientation accuracy bounded by a pitch and roll error of less than $3.0^\circ$ ($0.05236\text{ rad}$) RMS and a maximum instantaneous peak error of less than $5.0^\circ$ ($0.08726\text{ rad}$) across a continuous $10.0\text{-minute}$ ($600.0\text{ s}$) dynamic flight profile. | Scope § Success metrics; Finding F-HLR-01 | Test (HLT) |
| **HLR-PRF-004** | The system shall be verifiable against synthetically generated ground-truth trajectories pairing deterministic 6-DOF IMU samples with true orientation at each timestep. | Scope § Test data; DO-178C Table A-2 (Obj 2) | Test (HLT) |

---

## 3. Interface & Timing Requirements (HLR-IFC)

| Requirement ID | Statement | Rationale / Source | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-IFC-001** | The system shall ingest synchronized tri-axial specific force ($a_x, a_y, a_z$) in $\text{m/s}^2$ bounded within `||a|| <= 160.0 m/s^2` and tri-axial angular rates ($\omega_x, \omega_y, \omega_z$) in $\text{rad/s}$ bounded within `|ω| <= 35.0 rad/s` with a nominal epoch interval of $\Delta t = 0.01\text{ s}$ ($100\text{ Hz}$), discarding any sample containing non-finite values (`NaN` or `Inf`). | Scope § Sensors modeled; Finding F-HLR-03 | Test (HLT / Robustness) |
| **HLR-IFC-002** | The system shall provide non-blocking query interfaces returning the current estimated quaternion, Euler angles, and filter health status without modifying internal filter state. | `docs/standards/SDS.md` § Loose Coupling | Review / Test (HLT) |

---

## 4. Safety & Integrity Requirements (HLR-SAF) [Derived Requirements]

*Note: Requirements in this section are derived directly from the system safety assessment (`FHA_summary.md`) to mitigate Hazardously Misleading Information (FHA-AHRS-001).*

| Requirement ID | Statement | Parent Safety Allocation | Verification Method |
| :--- | :--- | :--- | :--- |
| **HLR-SAF-001** | The system shall reject and flag as invalid any sensor sample whose elapsed timestep satisfies $\Delta t \le 0.0\text{ s}$ or $\Delta t > 0.10\text{ s}$, retaining the last verified valid attitude state and asserting an invalid-sample status flag. | `SR-SAF-001` (FHA § 5); Finding F-HLR-04 | Test (HLT / Robustness) |
| **HLR-SAF-002** | The system shall guarantee that the Euclidean norm of the estimated attitude quaternion remains strictly bounded ($\vert{}\Vert{}q\Vert{} - 1.0\vert{} \le 1.0\times 10^{-6}$) after every prediction and update cycle. | `SR-SAF-002` (FHA § 5) | Test (HLT / Robustness) |
| **HLR-SAF-003** | The system shall reject accelerometer correction updates when the measured acceleration norm deviates from nominal gravity by more than $\pm 20\%$ ($\Vert{}a\Vert{} < 7.848\text{ m/s}^2$ or $\Vert{}a\Vert{} > 11.772\text{ m/s}^2$), sustaining attitude through pure gyroscopic propagation. | FHA-AHRS-001 (Prevents corrupted gravity vectors under dynamic linear acceleration) | Test (HLT / Robustness) |
| **HLR-SAF-004** | The system shall execute without dynamic runtime heap allocation (`malloc`, `free`, `new`, `delete`), utilizing statically bounded memory allocations. | `SR-SAF-003` (FHA § 5); `docs/standards/SCS.md` | Static Analysis / Inspection |

---

## 5. Requirements Review Checklist (DO-178C Table A-2 Verification)

| Item # | Verification Check Item | DO-178C Criteria Reference | Verification Method | Result (Pass / Fail / In Work) | Review Findings / Evidence |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-SRS-01** | **Mandatory Modality:** Does every requirement state normative intent using `shall` syntax, excluding ambiguous verbs (`should`, `may`, `will`)? | Section 5.1.2.a | Visual Inspection | **[X] Pass** | 100% compliance across all 14 HLRs; no non-normative auxiliary verbs detected. |
| **CHK-SRS-02** | **Absence of Ambiguity:** Is the statement free of qualitative/untestable terms (e.g., *fast*, *robust*, *optimized*, *TBD*)? | Section 5.1.2.b | Lexical Search / Peer Review | **[X] Pass** | Finding F-HLR-01 & F-HLR-02 closed: `HLR-PRF-003` numerical dynamic ceilings specified; `HLR-FNC-004` Tait-Bryan Z-Y-X convention and singularity clamping defined. |
| **CHK-SRS-03** | **Verifiability & Quantifiable Tolerances:** Does the requirement establish numerical thresholds, timing bounds, SI units, and deterministic limits? | Section 6.2.2.a | Review vs Scope Targets | **[X] Pass** | Finding F-HLR-01 & F-HLR-02 closed: `HLR-PRF-001` specifies observation window >= 10.0 s; `HLR-PRF-003` specifies dynamic error < 3.0° RMS and < 5.0° peak. |
| **CHK-SRS-04** | **Input/Output Domain Completeness:** Are nominal operating intervals, boundary/edge conditions, and invalid inputs explicitly addressed? | Section 5.1.2.b | Boundary Analysis Review | **[X] Pass** | Finding F-HLR-03 & F-HLR-04 closed: `HLR-IFC-001` specifies bounds (||a|| <= 160.0 m/s², |ω| <= 35.0 rad/s) & NaN/Inf sanitization; `HLR-SAF-001` specifies fallback state & status flag. |
| **CHK-SRS-05** | **Implementation Independence:** Does the HLR define functional intent without dictating programming language constructs or local variables? | Section 5.1.2.a | Architecture Decoupling Check | **[X] Pass** | Functional intent clearly separated from language implementation; memory allocation constraints in `HLR-SAF-004` reflect valid safety allocations. |
| **CHK-SRS-06** | **Traceability Integrity:** Is the requirement assigned an immutable identifier and registered bidirectionally in the RTM? | Section 5.5 / Table A-2 | Traceability Audit | **[X] Pass** | Finding F-HLR-05 closed: 100% bidirectional traceability between canonical HLR IDs (`HLR-FNC-*`, `HLR-PRF-*`, `HLR-IFC-*`, `HLR-SAF-*`) and LLRs in `traceability_matrix.md`. |
| **CHK-SRS-07** | **Derived Safety Identification:** If the requirement is derived (e.g., `HLR-SAF-*`), has it been formally flagged and reported to system safety? | Section 5.2.2 / FHA | Safety Assessment Check | **[X] Pass** | All derived safety requirements (`HLR-SAF-001..004`) are formally flagged and traced to `FHA_summary.md` (`SR-SAF-001..003`). |

### Review & Sign-off Record
* **Target Baseline:** `docs/requirements/HLR.md` (v1.1 Approved Candidate)
* **Author / Submitter:** Software Engineering Team | Date: 2026-09-29
* **Independent Reviewer (Verification Role):** Independent Airborne Software Verification Engineer (Sim) | Date: 2026-09-29
* **SQA Gatekeeper Approval:** SQA Gatekeeper Audit Role (Sim) — **Approved (Lock v0.2.0-SOI-2)** | Date: 2026-09-29
