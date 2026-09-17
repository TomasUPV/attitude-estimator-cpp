# Software Quality Assurance Plan (SQAP)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.5  
**Target Design Assurance Level:** DAL B  
**Document Version:** 0.1 (Draft)  
**Status:** In Work / Draft  

---

## 1. Overview & Objectives

This Software Quality Assurance Plan (SQAP) defines the independent oversight activities, evaluation processes, and audit mechanisms required to assure that all lifecycle processes and generated software products comply with approved plans and standards.

In accordance with RTCA DO-178C Section 8 and Table A-9 objectives, the primary goals are:
* Assure that software development and integral processes comply with approved plans (PSAC, SDP, SVP, SCMP).
* Verify that software lifecycle data meets governing engineering standards (SRS, SDS, SCS).
* Establish corrective action tracking for deviations, ensuring resolution prior to certification sign-off.

---

## 2. SQA Organization & Authority

To fulfill the independence requirements of DO-178C Table A-9:
* **Organizational Independence:** The SQA function operates with organizational independence from the development and testing team, reporting directly to quality leadership rather than engineering managers.
* **Authority to Halt Transitions:** SQA representatives hold the explicit authority to reject baseline milestones, halt pull request merges, and refuse transition authorizations if quality non-conformances are detected.
* **Responsibilities:**
  * Performing in-process audits of software requirements, design, code, and verification artifacts.
  * Verifying baseline integrity and configuration control adherence.
  * Attending and validating formal Stage of Involvement (SOI) audit packages.

---

## 3. SQA Activities & Evaluation Processes

```text
  [ Engineering Process Execution ]
                 │
                 ▼
  [ SQA In-Process Milestone Review ] ──► [ Non-Conformance Detected? ]
                 │                                      │
                 │ (Clean / Conforming)                 ▼ (Yes)
                 │                       [ Quality Action Item (QAI) Logged ]
                 │                                      │
                 ▼                                      ▼
  [ SQA Approval for Lifecycle Transition ]   [ Corrective Action Verified ]
  ```
### 3.1 Planning Process Assurance (SOI-1)
SQA verifies that:
* The PSAC, SDP, SVP, SCMP, and SQAP are mutually consistent and cover all applicable DO-178C DAL B objectives.
* Engineering standards (SRS, SDS, SCS) and the tool environment configuration index (SECI) are baselined under configuration control before development commences.

### 3.2 Requirements & Design Process Assurance (SOI-2)
SQA evaluates that:
* High-Level Requirements comply with the syntax, testability, and determinism rules of `docs/standards/SRS.md`.
* Modular architecture descriptions in `docs/design/*.md` adhere to `docs/standards/SDS.md`.
* The bidirectional Requirements Traceability Matrix (`traceability_matrix.md`) maintains zero untraced requirements.

### 3.3 Implementation & Verification Assurance (SOI-3)
SQA inspects that:
* Source code in `src/` and `include/` complies strictly with `docs/standards/SCS.md` (no dynamic memory, explicit type widths, defensive checks).
* Automated CI pipeline execution logs (`.github/workflows/ci.yml`) pass with zero warnings under `-Werror`, `cppcheck`, and `clang-tidy`.
* Requirements-based test suites in `tests/` exercise both nominal conditions and robustness boundary scenarios.
* Structural coverage analysis achieves 100% statement and 100% decision (branch) coverage for DAL B.

---

## 4. Software Conformity Review (SCR) & SOI-4 Closeout

Prior to final software delivery and Stage of Involvement 4 (SOI-4) audit:
* **Conformity Review Execution:** SQA conducts a comprehensive physical and functional audit to confirm that the executable software matches the baselined Git commit hash and release tag.
* **Verification Completeness:** Confirmation that 100% of planned verification activities are executed and closed with passing evidence.
* **Open Problem Reports Review:** SQA reviews the PR database to certify that zero unresolved Open Problem Reports (OPRs) affect safety or baseline stability.
* **Software Accomplishment Summary Sign-off:** Formal countersignature of the `docs/verification/SAS.md` closeout deliverable.

---

## 5. Corrective Action & Non-Conformance Tracking

When an audit identifies a process deviation or standard violation:
1. **Logging:** SQA records a formal Quality Action Item (QAI) referencing the non-conforming artifact and violated plan section.
2. **Root Cause Analysis:** The responsible developer performs an impact assessment and implements corrective measures.
3. **Closure Verification:** SQA re-audits the artifact to verify resolution before the action item is marked closed.
4. **Transition Gating:** No lifecycle milestone transition (SOI-1 through SOI-4) shall proceed while open SQA non-conformances remain against relevant baseline deliverables.

---

## 6. SQA Records & Audit Trail

SQA maintains controlled records of all quality oversight activities:
* Audit review checklists and peer inspection logs.
* Continuous integration verification summaries and static analysis logs.
* SQA formal sign-off records gating baseline tags (`v0.1.0-SOI-1`, `v0.2.0-SOI-2`, `v0.3.0-SOI-3`, `v1.0.0-SOI-4`).
* All quality records are archived under version control alongside technical lifecycle artifacts in compliance with DO-178C data retention requirements.
