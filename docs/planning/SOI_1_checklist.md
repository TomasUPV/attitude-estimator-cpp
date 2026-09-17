# Stage of Involvement 1 (SOI-1) Audit Checklist & Sign-Off Record

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 9.1 & 10.1  
**Target Design Assurance Level:** DAL B  
**Audit Stage:** Stage of Involvement 1 (Planning & Lifecycle Definition)  
**Audit Status:** Submitted for Review / Pending Audit  
**Submission Date:** 2026-09-17  

---

## 1. Scope & Objective of Audit

The primary objective of the Stage of Involvement 1 (SOI-1) review is to evaluate the completeness, consistency, and regulatory compliance of the software lifecycle planning documents and technical standards prior to authorizing software development activities.

This audit evaluates whether the applicant has:
* Formally defined the system context and justified the allocated Design Assurance Level (DAL B).
* Authored and baselined the mandatory lifecycle plans (PSAC, SDP, SVP, SCMP, SQAP).
* Established and baselined engineering standards (SRS, SDS, SCS) ensuring deterministic and safe coding practices.
* Formally frozen the development and verification toolchain in the SECI.

---

## 2. Document Baseline Submission Matrix

| Document Category | Document Name | File Path | Baseline Version | Review Status |
| :--- | :--- | :--- | :--- | :--- |
| **System Safety** | Functional Hazard Assessment | `docs/planning/FHA_summary.md` | v1.0 | Submitted for Review |
| **Regulatory Contract** | Plan for Software Aspects of Cert. | `docs/planning/PSAC.md` | v1.0 | Submitted for Review |
| **Lifecycle Plan** | Software Development Plan | `docs/planning/SDP.md` | v1.0 | Submitted for Review |
| **Lifecycle Plan** | Software Verification Plan | `docs/planning/SVP.md` | v1.0 | Submitted for Review |
| **Lifecycle Plan** | Software Configuration Mgmt. Plan | `docs/planning/SCMP.md` | v1.0 | Submitted for Review |
| **Lifecycle Plan** | Software Quality Assurance Plan | `docs/planning/SQAP.md` | v1.0 | Submitted for Review |
| **Technical Standard** | Software Requirements Standard | `docs/standards/SRS.md` | v1.0 | Submitted for Review |
| **Technical Standard** | Software Design Standard | `docs/standards/SDS.md` | v1.0 | Submitted for Review |
| **Technical Standard** | Software Code Standard | `docs/standards/SCS.md` | v1.0 | Submitted for Review |
| **Environment Control** | Software Environment Config. Index | `docs/planning/SECI.md` | v1.0 | Submitted for Review |
---

## 3. DO-178C Table A-1 Compliance Checklist

| Item # | Verification Criteria | Reference | Finding / Auditor Notes | Audit Result (Pass / Fail / Pending) |
| :--- | :--- | :--- | :--- | :--- |
| **CHK-SOI1-01** | Is the DAL allocation (DAL B) justified against system safety objectives? | DO-178C Sec 2.0 / FHA | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-02** | Does the PSAC define all required DO-178C Annex A Table objectives? | DO-178C Sec 11.1 / PSAC | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-03** | Does the SDP specify a deterministic life cycle and transition criteria? | DO-178C Sec 11.2 / SDP | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-04** | Does the SVP define requirements-based testing and coverage criteria? | DO-178C Sec 11.3 / SVP | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-05** | Does the SCMP define change control, baseline tagging, and branching? | DO-178C Sec 11.4 / SCMP | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-06** | Does the SQAP establish independent QA audits and non-conformance logs? | DO-178C Sec 11.5 / SQAP | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-07** | Are standards established for requirements, design, and code? | DO-178C Sec 11.6-11.8 | [To be evaluated by Auditor] | Pending Evaluation |
| **CHK-SOI1-08** | Is the development and verification toolchain frozen and reproducible? | DO-178C Sec 11.15 / SECI | [To be evaluated by Auditor] | Pending Evaluation |

---

## 4. Formal Audit Finding & Gate Authorization

### 4.1 Summary of Findings
* **Open Problem Reports (OPRs):** [Pending Auditor Input]
* **Lifecycle Plan Deficiencies:** [Pending Auditor Input]
* **Deviations / Action Items:** [Pending Auditor Input]

### 4.2 Authorization Statement
[ ] **APPROVED:** Authorization granted to transition from Planning into Development & Architecture (SOI-2).  
[ ] **REVISE & RESUBMIT:** Action items identified; updates required prior to lifecycle gate authorization.

### 4.3 Sign-Off Records
* **Software Engineering Lead:** Tomás Herrero Valero | Date: 2026-09-17
* **Software Quality Assurance Auditor:** _________________________ | Date: ____________
* **Certification Authority / DER Representative:** _________________ | Date: ____________
