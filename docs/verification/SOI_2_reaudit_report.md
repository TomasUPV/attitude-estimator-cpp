# Stage of Involvement 2 (SOI-2) — Re-Audit & Baseline Lock Checklist

> [!NOTE]
> **SIMULATION & PROCESS TESTING DOCUMENT**  
> This document is **NOT a formal certification record nor an official regulatory document**. It is an internal **simulation and process dry-run exercise**, specifically designed to test documentation structure, verify methodological consistency, and validate software lifecycle workflows under RTCA DO-178C / EUROCAE ED-12C principles.

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard Reference:** RTCA DO-178C / EUROCAE ED-12C (Simulated Context)  
**Target Design Assurance Level (Mock):** DAL B  
**Exercise Type:** Stage of Involvement 2 (SOI-2) Re-Audit & Baseline Lock  
**Simulation Status:** SIMULATION PASSED / SOI-2 Gate Cleared  
**Baseline Version:** 1.1  
**Status:** Released / SOI-2 Approved  
**Submission Date:** 2026-09-28  
**Simulation Date:** 2026-09-29  

---

## 1. Scope & Objective of the Simulation Exercise

The primary objective of this Stage of Involvement 2 (SOI-2) re-audit is to simulate an independent verification audit workflow under RTCA DO-178C Section 5.1 (Software Requirements Process), Section 5.2 (Software Design Process), Table A-2, and Table A-3.

This re-audit evaluates whether:
* The 9 discrepancies (`F-HLR-01` through `F-HLR-05` and `F-ARC-01` through `F-ARC-04`) identified during the preliminary dry-run recorded in `docs/verification/SOI_2_peer_review_report.md` have been fully resolved.
* High-Level Requirements (`HLR.md`) are unambiguous, quantifiable, verifiable, and adhere to normative `shall` auxiliary grammar.
* Bidirectional traceability between canonical HLRs and Low-Level Requirements (`LLR-*`) is 100% complete and verified in `traceability_matrix.md`, with zero legacy identifiers (`HLR-001`..`HLR-009`) remaining.
* The Software Architecture Description (`architecture.md`) allocates defensive safety invariants (gravity plausibility window, stale data fault containment mitigating `FHA-AHRS-001`), enforces memory determinism (zero runtime heap, bounded stack), and specifies unambiguous Row-Major matrix memory layout.

---

## 2. Simulated Document Baseline Submission Matrix

| Document Category | Document Name | File Path | Baseline Version | Simulation Review Status |
| :--- | :--- | :--- | :--- | :--- |
| **Requirements** | High-Level Requirements (HLR) | `docs/requirements/HLR.md` | v1.1 | Tested & Validated (Simulated) |
| **Architecture** | Software Architecture Description (SAD) | `docs/design/architecture.md` | v1.1 | Tested & Validated (Simulated) |
| **Traceability** | Requirements Traceability Matrix (RTM) | `docs/requirements/traceability_matrix.md` | v1.1 | Tested & Validated (Simulated) |
| **Module Design** | QuaternionMath Low-Level Design | `docs/design/quaternion_math.md` | v1.1 | Tested & Validated (Simulated) |
| **Module Design** | SensorModel Low-Level Design | `docs/design/sensor_model.md` | v1.1 | Tested & Validated (Simulated) |
| **Module Design** | EKF_Core Low-Level Design | `docs/design/ekf_core.md` | v1.1 | Tested & Validated (Simulated) |
| **Module Design** | AttitudeEstimator Facade Design | `docs/design/attitude_estimator.md` | v1.1 | Tested & Validated (Simulated) |
| **Historical Record** | Prior SOI-2 Peer Review Dry-Run | `docs/verification/SOI_2_peer_review_report.md` | v1.0 | Historical Baseline (Preserved) |

---

## 3. DO-178C Table A-2 & Table A-3 Simulated Compliance Checklists

### 3.1 DO-178C Table A-2 (Software Requirements Process)

| Item # | Verification Criteria | Reference | Simulation Notes / Findings | Result (Dry-Run) |
| :--- | :--- | :--- | :--- | :--- |
| **CHK-SRS-01** | Mandatory Modality | DO-178C Sec 5.1.2.a | 100% compliance across all 14 HLRs; exclusively normative `shall` syntax utilized. | **Passed (Simulated)** |
| **CHK-SRS-02** | Absence of Ambiguity | DO-178C Sec 5.1.2.b | Free of colloquialisms. `HLR-PRF-003` defines explicit numerical dynamic ceilings; `HLR-FNC-004` specifies Tait-Bryan Z-Y-X sequence and singularity bounds. | **Passed (Simulated)** |
| **CHK-SRS-03** | Verifiability & Quantifiable Tolerances | DO-178C Sec 6.2.2.a | `HLR-PRF-001` establishes static observation window $\ge 10.0\text{ s}$; `HLR-PRF-003` quantifies dynamic bounds ($< 3.0^\circ$ RMS, $< 5.0^\circ$ peak across $10\text{ min}$). | **Passed (Simulated)** |
| **CHK-SRS-04** | Input/Output Domain Completeness | DO-178C Sec 5.1.2.b | `HLR-IFC-001` defines ingress bounds ($\|a\| \le 160.0\text{ m/s}^2$, $\|\omega\| \le 35.0\text{ rad/s}$) & non-finite rejection; `HLR-SAF-001` specifies fallback state & status flag. | **Passed (Simulated)** |
| **CHK-SRS-05** | Implementation Independence | DO-178C Sec 5.1.2.a | Functional requirements describe behavior without source constructs. Memory constraints in `HLR-SAF-004` reflect valid safety allocations. | **Passed (Simulated)** |
| **CHK-SRS-06** | Traceability Integrity | DO-178C Sec 5.5 / Table A-2 | 100% bidirectional traceability between canonical HLR IDs (`HLR-FNC-*`, `HLR-PRF-*`, `HLR-IFC-*`, `HLR-SAF-*`) and LLRs in `traceability_matrix.md`. | **Passed (Simulated)** |
| **CHK-SRS-07** | Derived Safety Identification | DO-178C Sec 5.2.2 / FHA | All derived safety requirements (`HLR-SAF-001..004`) are formally flagged and traced to `FHA_summary.md` (`SR-SAF-001..003`). | **Passed (Simulated)** |

### 3.2 DO-178C Table A-3 (Software Architecture Process)

| Item # | Verification Criteria | Reference | Simulation Notes / Findings | Result (Dry-Run) |
| :--- | :--- | :--- | :--- | :--- |
| **CHK-ARC-01** | Compatibility with HLR | DO-178C Table A-3 (Obj 1) | Invariant 4 in `architecture.md` allocates `HLR-SAF-003` ($[7.848, 11.772]\text{ m/s}^2$ plausibility window) with pure gyroscopic fallback. | **Passed (Simulated)** |
| **CHK-ARC-02** | Consistency with SDS | DO-178C Table A-3 (Obj 2) | Architecture strictly adheres to `namespace attitude`, `using float64_t = double;` (`SDS.md` §3.2 & `SCS.md` §2.1), and harmonized `SensorModel` data structs. | **Passed (Simulated)** |
| **CHK-ARC-03** | Deterministic Execution & Memory | DO-178C Table A-3 (Obj 3) | Invariant 1 verified: Zero runtime dynamic heap allocation (`malloc`/`new`), no recursion, compile-time static `std::array`, and bounded stack depth. | **Passed (Simulated)** |
| **CHK-ARC-04** | Defined Interfaces & Data Flow | DO-178C Table A-3 (Obj 4) | Section 3 of `architecture.md` explicitly specifies contiguous Row-Major layout and indexing formula $k = i \cdot N + j$ for `Matrix4x4` and `Matrix3x3`. | **Passed (Simulated)** |
| **CHK-ARC-05** | Safety Invariants & Partition Boundaries | DO-178C Table A-3 (Obj 5) | Invariant 6 enforces stale-data mitigation ($N_{drop} > 10$ asserts `STATUS_STALE_DATA`) and `LLR-AE-007` implements containment against Hazard `FHA-AHRS-001` (HMI). | **Passed (Simulated)** |

---

## 4. Simulation Findings & Process Gate Authorization

### 4.1 Summary of Observations & Closure Log

* **Prior Dry-Run Discrepancies:** 9 Findings (`F-HLR-01` through `F-HLR-05` and `F-ARC-01` through `F-ARC-04`).
* **Verified Closed Items:** 9 / 9 Findings (100% Closure Rate).
* **Open Problem Reports (OPRs):** 0 Open Findings.

| Finding ID | Artifact | DO-178C Objective | Corrective Verification Evidence | Closure Status |
| :--- | :--- | :--- | :--- | :---: |
| **F-HLR-01** | `HLR.md` | Table A-2 (Obj 2 & 4) | `HLR-PRF-003` defines dynamic ceilings: $< 3.0^\circ$ RMS and $< 5.0^\circ$ peak across $10.0\text{-minute}$ dynamic profile. | **CLOSED** |
| **F-HLR-02** | `HLR.md` | Table A-2 (Obj 2 & 7) | `HLR-FNC-004` specifies Tait-Bryan $Z-Y-X$ convention ($\psi \to \theta \to \phi$) with singularity clamping; `HLR-PRF-001` specifies observation window $\ge 10.0\text{ s}$. | **CLOSED** |
| **F-HLR-03** | `HLR.md` | Table A-2 (Obj 4) | `HLR-IFC-001` specifies ingress limits ($\|a\| \le 160.0\text{ m/s}^2$, $\|\omega\| \le 35.0\text{ rad/s}$) and mandatory non-finite (`NaN`/`Inf`) rejection. | **CLOSED** |
| **F-HLR-04** | `HLR.md` | Table A-2 (Obj 2) | `HLR-SAF-001` specifies retaining last verified state and asserting an invalid-sample status flag upon timestep violations. | **CLOSED** |
| **F-HLR-05** | `traceability_matrix.md` | Table A-2 (Obj 6) | 100% bidirectional traceability between canonical HLR IDs and LLRs; legacy IDs (`HLR-001`..`HLR-009`) purged across all specifications. | **CLOSED** |
| **F-ARC-01** | `architecture.md` | Table A-3 (Obj 1) | Invariant 4 allocates `HLR-SAF-003` ($[7.848, 11.772]\text{ m/s}^2$) with gyroscopic fallback. Traced to `LLR-AE-002` and `LLR-EKF-004`. | **CLOSED** |
| **F-ARC-02** | `architecture.md` | Table A-3 (Obj 2 & 5) | `namespace attitude`, `using float64_t = double;`, and harmonized data contracts (`ImuSample`, `AccelReading`, `GyroReading`). | **CLOSED** |
| **F-ARC-03** | `architecture.md` | Table A-3 (Obj 2 & 4) | Row-Major memory layout and index rule ($k = i \cdot N + j$) explicitly specified for `Matrix4x4` and `Matrix3x3`. | **CLOSED** |
| **F-ARC-04** | `architecture.md` | Table A-3 (Obj 5) | Invariant 6 and `LLR-AE-007` implement stale data counter $N_{drop} > 10$ asserting `STATUS_STALE_DATA` against Hazard `FHA-AHRS-001`. | **CLOSED** |

### 4.2 Dry-Run Gate Authorization

[X] **APPROVED (SIMULATION):** Process validation and finding verification complete; authorized to transition into Low-Level Requirements (LLR) formalization and C++17 airborne software source implementation.  
[ ] **REVISE & RESUBMIT:** Further iteration needed prior to gate progression.

*Simulation Summary Statement:*  
The Stage of Involvement 2 (SOI-2) re-audit simulation for the **Attitude Estimator (EKF Pitch/Roll Core)** component has been completed as an internal dry-run exercise. All 9 findings from the preliminary audit have been verified as closed with objective technical evidence. High-Level Requirements, Software Architecture, and Traceability baselines satisfy DO-178C Table A-2 and Table A-3 objectives for DAL B. The technical baseline is officially locked under configuration identifier **`Lock v0.2.0-SOI-2`**.

### 4.3 Sign-Off Records

* **Software Engineering Lead:** Tomás Herrero Valero | Date: 2026-09-29
* **Software Quality Assurance (Simulated Review):** AI SQA Simulation Auditor | Date: 2026-09-29
* **Certified by:** Antigravity (AI Verification Simulation Auditor / Process Auditor) | Date: 2026-09-29
