# Stage of Involvement 1 (SOI-1) — Simulation & Process Dry-Run Checklist

> [!NOTE]
> **SIMULATION & PROCESS TESTING DOCUMENT**  
> This document is **NOT a formal certification record nor an official regulatory document**. It is an internal **simulation and process dry-run exercise**, specifically designed to test documentation structure, verify methodological consistency, and validate software lifecycle workflows under RTCA DO-178C / EUROCAE ED-12C principles.

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard Reference:** RTCA DO-178C / EUROCAE ED-12C (Simulated Context)  
**Target Design Assurance Level (Mock):** DAL B  
**Exercise Type:** Stage of Involvement 1 (SOI-1) Dry-Run & Process Validation  
**Simulation Status:** SIMULATION PASSED / Dry-Run Gate Cleared  
**Baseline Version:** 1.0  
**Status:** Released / SOI-1 Baseline  
**Submission Date:** 2026-09-17  
**Simulation Date:** 2026-09-18  

---

## 1. Scope & Objective of the Simulation Exercise

The primary objective of this Stage of Involvement 1 (SOI-1) dry-run is to simulate an audit workflow to test, exercise, and validate the software lifecycle planning documents and technical standards before moving on to subsequent development activities.

This simulation evaluates whether:
* The system context and simulated DAL B allocation are clearly articulated.
* The mandatory lifecycle planning documents (PSAC, SDP, SVP, SCMP, SQAP) have been drafted and tested for structural consistency.
* Engineering standards (SRS, SDS, SCS) are established to promote deterministic and safe coding practices.
* The development, compilation, and verification toolchain is documented and frozen in the SECI.

---

## 2. Simulated Document Baseline Submission Matrix

| Document Category | Document Name | File Path | Baseline Version | Simulation Review Status |
| :--- | :--- | :--- | :--- | :--- |
| **System Safety** | Functional Hazard Assessment | `docs/planning/FHA_summary.md` | v1.0 | Tested & Validated (Simulated) |
| **Regulatory Contract** | Plan for Software Aspects of Cert. | `docs/planning/PSAC.md` | v1.0 | Tested & Validated (Simulated) |
| **Lifecycle Plan** | Software Development Plan | `docs/planning/SDP.md` | v1.0 | Tested & Validated (Simulated) |
| **Lifecycle Plan** | Software Verification Plan | `docs/planning/SVP.md` | v1.0 | Tested & Validated (Simulated) |
| **Lifecycle Plan** | Software Configuration Mgmt. Plan | `docs/planning/SCMP.md` | v1.0 | Tested & Validated (Simulated) |
| **Lifecycle Plan** | Software Quality Assurance Plan | `docs/planning/SQAP.md` | v1.0 | Tested & Validated (Simulated) |
| **Technical Standard** | Software Requirements Standard | `docs/standards/SRS.md` | v1.0 | Tested & Validated (Simulated) |
| **Technical Standard** | Software Design Standard | `docs/standards/SDS.md` | v1.0 | Tested & Validated (Simulated) |
| **Technical Standard** | Software Code Standard | `docs/standards/SCS.md` | v1.0 | Tested & Validated (Simulated) |
| **Environment Control** | Software Environment Config. Index | `docs/planning/SECI.md` | v1.0 | Tested & Validated (Simulated) |

---

## 3. DO-178C Table A-1 Simulated Compliance Checklist

| Item # | Verification Criteria | Reference | Simulation Notes / Findings | Result (Dry-Run) |
| :--- | :--- | :--- | :--- | :--- |
| **CHK-SOI1-01** | Is the DAL allocation (DAL B) justified against system safety objectives? | DO-178C Sec 2.0 / FHA | Justified by ARP4761 FHA-AHRS-001 (Hazardous / HMI failure condition). Dual DAL B / DAL C strategy clearly documented. | **Passed (Simulated)** |
| **CHK-SOI1-02** | Does the PSAC define all required DO-178C Annex A Table objectives? | DO-178C Sec 11.1 / PSAC | PSAC covers Annex A Tables A-1 to A-10 for DAL B, establishes verification independence, and addresses DO-330 tool qualification. | **Passed (Simulated)** |
| **CHK-SOI1-03** | Does the SDP specify a deterministic life cycle and transition criteria? | DO-178C Sec 11.2 / SDP | Classical V-Model defined with strict transition gates, C++17 determinism rules (no dynamic memory, static stack), and clear entry/exit criteria. | **Passed (Simulated)** |
| **CHK-SOI1-04** | Does the SVP define requirements-based testing and coverage criteria? | DO-178C Sec 11.3 / SVP | Defines LLT/HLT RBT using GoogleTest and 100% Statement + Decision (Branch) Coverage for DAL B. Includes regression protocols. | **Passed (Simulated)** |
| **CHK-SOI1-05** | Does the SCMP define change control, baseline tagging, and branching? | DO-178C Sec 11.4 / SCMP | Git branching strategy (`main`, `develop`, `feature/*`, `bugfix/*`), protected main branch, semver tags (`v0.1.0-SOI-1`), and PR tracking defined. | **Passed (Simulated)** |
| **CHK-SOI1-06** | Does the SQAP establish independent QA audits and non-conformance logs? | DO-178C Sec 11.5 / SQAP | SQA organizational independence established with explicit authority to halt gate transitions. Formal QAI tracking and SCR defined. | **Passed (Simulated)** |
| **CHK-SOI1-07** | Are standards established for requirements, design, and code? | DO-178C Sec 11.6-11.8 | SRS, SDS, SCS fully articulated with quantifiable rules (no heap, standard header guards, `noexcept`, `float64_t`, division epsilon, peer checklists). | **Passed (Simulated)** |
| **CHK-SOI1-08** | Is the development and verification toolchain frozen and reproducible? | DO-178C Sec 11.15 / SECI | SECI freezes CMake 3.22+, Clang 14+, GCC 11+, GoogleTest 1.14.0, Cppcheck, Clang-Tidy, and exact compiler flags with strict change control. | **Passed (Simulated)** |

---

## 4. Simulation Findings & Process Gate Authorization

### 4.1 Summary of Observations
* **Simulated Open Problem Reports (OPRs):** 0 OPRs.
* **Process / Lifecycle Plan Deficiencies:** 0 Critical Deficiencies.
* **Observations & Process Recommendations:**
  * **OBS-SOI1-01 (Version Identifier Synchronization):** Align inner document header baseline versions (`0.1 (Draft)`) with submission baseline tag `v1.0` / `v0.1.0-SOI-1` prior to SOI-2 tagging. **[CLOSED — Synchronized to v1.0 / Released / SOI-1 Baseline]**
  * **OBS-SOI1-02 (Compiler Flag Enhancements):** Consider explicitly adding `-fno-fast-math` and `-Wdouble-promotion` to CMake build configurations to prevent implicit floating-point conversions.
  * **OBS-SOI1-03 (Automated Traceability Validation):** Recommend adding a CI automated script to validate 100% bidirectional traceability in `docs/requirements/traceability_matrix.md` prior to SOI-2.

### 4.2 Dry-Run Gate Authorization
[X] **APPROVED (SIMULATION):** Process validation complete; authorized to transition into Development & Architecture dry-run (SOI-2).  
[ ] **REVISE & RESUBMIT:** Further iteration needed prior to gate progression.

*Simulation Summary Statement:*  
The Stage of Involvement 1 (SOI-1) process simulation for the **Attitude Estimator (EKF Pitch/Roll Core)** component has been completed as an internal dry-run exercise. The tested lifecycle plans, safety assessments, standards, and environment configuration indices successfully demonstrate the intended DO-178C DAL B process structure. The engineering workflow is approved to proceed to **Phase 2: Requirements Definition & Architecture Specification (SOI-2 Simulation)**.

### 4.3 Sign-Off Records
* **Software Engineering Lead:** Tomás Herrero Valero | Date: 2026-09-17
* **Software Quality Assurance (Simulated Review):** Internal SQA Process Reviewer | Date: 2026-09-17
* **Certified by:** Antigravity (Simulated Certification Authority / Process Auditor) | Date: 2026-09-18


