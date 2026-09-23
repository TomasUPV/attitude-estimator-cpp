# Software Requirements Standard (SRS)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.6  
**Target Design Assurance Level:** DAL B  
**Baseline Version:** 1.0  
**Status:** Released / SOI-1 Baseline  

---

## 1. Scope & Objective

This Software Requirements Standard (SRS) defines the mandatory syntax, formulation rules, structural criteria, and quality constraints for authoring High-Level Requirements (HLRs) and Low-Level Requirements (LLRs) for the Attitude Estimator component.

In accordance with RTCA DO-178C Section 5.1 and Section 11.6, this standard ensures that all requirements are:
* Unambiguous, consistent, and mathematically verifiable.
* Traceable bidirectionally across the software development lifecycle.
* Deterministic and free of implementation bias.

---

## 2. Requirement Identification & Formatting

### 2.1 Unique Identifier Scheme
Every requirement shall be assigned an immutable, unique identifier following standard nomenclature:
* **High-Level Requirements:** `HLR-<CATEGORY>-<INDEX>` (e.g., `HLR-PRF-001`, `HLR-SAF-002`).
* **Low-Level Requirements:** `LLR-<MODULE>-<INDEX>` (e.g., `LLR-QM-001`, `LLR-EKF-003`).

### 2.2 Prescribed Grammar & Modality
Requirements shall be written in clear, concise English using strict normative phrasing:
* **`shall`**: Indicates a mandatory requirement subject to formal verification and compliance auditing.
* **`should`**: Indicates design guidance or non-mandatory objectives; strictly forbidden within normative HLR/LLR statements.
* **`will`**: Reserved solely for operational facts or environmental characteristics outside software control.

### 2.3 Forbidden Ambiguity Phrasing
Requirement statements shall avoid qualitative, untestable, and subjective terms, including:
* *Fast, slow, user-friendly, optimized, efficient, robust, flexible, approximately, if feasible, TBD, etc.*

---

## 3. Structural & Content Criteria

Every requirement baseline shall satisfy the four fundamental criteria defined in DO-178C Section 5.1.2:

### 3.1 Verifiability & Testability
* Each requirement shall specify quantifiable pass/fail criteria, numeric tolerances, and bounded operating ranges.
* Statements specifying physical quantities shall explicitly include SI units (e.g., `rad`, `s`, `deg/s`, `m/s²`).

### 3.2 Determinism & Range Definition
* Requirements governing computational steps shall specify behavior across the entire operational domain, including nominal input intervals, abnormal inputs, boundary values, and degenerate states (e.g., division by zero or sensor saturation).

### 3.3 Implementation Independence (HLR Specific)
* High-Level Requirements shall specify *what* the functional software shall achieve without dictating algorithmic implementation details, C++ constructs, or CPU register manipulations.

### 3.4 Bidirectional Traceability Governance
* Every HLR shall trace to at least one system-level safety or operational allocation in `FHA_summary.md` or `scope.md`.
* Every LLR shall trace backward to a parent HLR and forward to concrete source functions and GoogleTest fixtures.

---

## 4. Derived Requirements Management

* **Definition:** Requirements produced during the software design process that do not trace directly from a parent requirement are classified as *Derived Requirements*.
* **Safety Review:** All derived requirements (such as algorithmic normalization thresholds or matrix singularity recovery paths) shall be explicitly documented and submitted to system safety engineering for impact assessment (DO-178C Section 5.2.2).

---
---

## 5. Requirement Review Checklist (Peer Inspection & SQA Gate)

Prior to baselining any requirement data item (`HLR.md` or modular LLRs in `docs/design/*.md`), the independent reviewer and SQA auditor shall execute and sign this verification checklist:

| Item # | Verification Check Item | DO-178C Criteria Reference | Verification Method | Result (Pass / Fail / N/A) | Evidence / Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-SRS-01** | **Mandatory Modality:** Does every requirement state normative intent using `shall` syntax, excluding ambiguous verbs (`should`, `may`, `will`)? | Section 5.1.2.a | Visual Inspection | | |
| **CHK-SRS-02** | **Absence of Ambiguity:** Is the statement free of qualitative/untestable terms (e.g., *fast*, *robust*, *optimized*, *TBD*)? | Section 5.1.2.b | Lexical Search / Peer Review | | |
| **CHK-SRS-03** | **Verifiability & Quantifiable Tolerances:** Does the requirement establish numerical thresholds, timing bounds, SI units, and deterministic pass/fail limits? | Section 6.2.2.a | Review vs Scope Targets | | |
| **CHK-SRS-04** | **Input/Output Domain Completeness:** Are nominal operating intervals, boundary/edge conditions, and invalid inputs explicitly addressed? | Section 5.1.2.b | Boundary Analysis Review | | |
| **CHK-SRS-05** | **Implementation Independence (HLR only):** Does the HLR define functional intent without dictating programming language constructs, local variables, or CPU instructions? | Section 5.1.2.a | Architecture Decoupling Check | | |
| **CHK-SRS-06** | **Traceability Integrity:** Is the requirement assigned an immutable identifier and registered bidirectionally in `docs/requirements/traceability_matrix.md`? | Section 5.5 / Table A-3 | Traceability Audit | | |
| **CHK-SRS-07** | **Derived Safety Identification:** If the requirement is derived (no parent requirement), has it been formally flagged and reported to system safety engineering? | Section 5.2.2 | Safety Assessment Check | | |

### Checklist Sign-off Record
* **Review Target Artifact:** `docs/requirements/HLR.md` / `docs/design/*.md`
* **Artifact Baseline Version:** _______________
* **Author / Submitter:** ______________________  | Date: ____________
* **Independent Peer Reviewer:** ______________  | Date: ____________
* **SQA Gatekeeper Approval:** ________________  | Date: ____________
