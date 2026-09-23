# Software Configuration Management Plan (SCMP)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.4  
**Target Design Assurance Level:** DAL B  
**Document Version:** 0.1 (Draft)  
**Status:** In Work / Draft  

---

## 1. Overview & Objectives

This Software Configuration Management Plan (SCMP) establishes the mechanisms, processes, and controls required to maintain the integrity and reproducibility of all software lifecycle data for the Attitude Estimator component.

In accordance with RTCA DO-178C Section 7 and Table A-8 objectives, this plan defines:
* Configuration identification and baseline management.
* Change control, branch protection, and problem reporting workflows.
* Archive, retrieval, and environment reproducibility controls.

---

## 2. Configuration Management Organization & Responsibilities

Configuration management is implemented via Git version control and GitHub platform governance:
* **Configuration Manager (CM Lead):** Oversees baseline tagging, release branching, and merge authorizations into protected branches.
* **Software Engineers (Developers):** Responsible for initiating isolated topic branches, authoring atomic commits with descriptive messages, and raising pull requests.
* **Independent Reviewers / QA:** Verify that every change traces directly to an approved requirement or Problem Report prior to baseline integration.

---

## 3. Configuration Identification & Baselining

### 3.1 Configuration Items (CIs)
Every file under the repository tree is classified as a controlled Configuration Item:
* **Planning & Standards:** `docs/planning/*.md`, `docs/standards/*.md`.
* **Requirements & Design:** `docs/requirements/*.md`, `docs/design/*.md`.
* **Source & Build Files:** `include/**/*.h`, `src/**/*.cpp`, `CMakeLists.txt`.
* **Verification Artifacts:** `tests/**/*.cpp`, `.clang-tidy`, `.github/workflows/*.yml`.

### 3.2 Branching Strategy & Git Governance
Development strictly follows a structured branching model:
* **`main` (Baselined Trunk):** Contains only verified, approved release baselines. Direct commits are forbidden. Merging requires passing CI builds and manual peer sign-off.
* **`develop` (Integration Trunk):** Continuous integration branch where completed feature branches converge.
* **`feature/<ID>-<description>`:** Short-lived branches dedicated to a specific Low-Level Requirement or modular component (e.g., `feature/LLR-QM-001-hamilton-product`).
* **`bugfix/<PR-ID>-<description>`:** Branches created to resolve verified Problem Reports.

### 3.3 Baseline Identification & Semantic Versioning
Baselines represent frozen lifecycle milestones tagged with immutable Git release tags following semantic versioning (`vMAJOR.MINOR.PATCH`):
* **`v0.1.0-SOI-1`:** Baseline for Planning documents and Engineering Standards.
* **`v0.2.0-SOI-2`:** Baseline for High-Level Requirements, Architecture, and Low-Level Requirements.
* **`v0.3.0-SOI-3`:** Baseline for Source Code, Static Analysis pass, and Unit Verification Suites.
* **`v1.0.0-SOI-4`:** Final flight-candidate release baseline containing closed traceability and SAS summary.
---

## 4. Change Control & Problem Reporting Workflow

In accordance with DO-178C Section 7.2.3, no modification to a baselined configuration item shall occur without formal change evaluation and recording.

```text
  [ Problem / Change Identified ]
                 │
                 ▼
  [ Formal Problem Report (PR) Logged ]
                 │
                 ▼
  [ Impact Analysis (Upstream HLR & Downstream Tests) ]
                 │
                 ▼
  [ Feature / Bugfix Branch Execution ]
                 │
                 ▼
  [ CI Automated Validation (Build + Tests + Linters) ]
                 │
                 ▼
  [ Peer Review Approval & Merge to Baseline ]
```
### 4.1 Problem Reporting (PR) Process
1. Any defect, test failure, or ambiguity in requirements is recorded as a Problem Report (GitHub Issue tracking).
2. The PR is assigned a unique tracking identifier (`PR-XXX`), severity classification, and component target.
3. Open Problem Reports (OPRs) are tracked continuously; zero unclosed OPRs are permitted for final SOI-4 certification closeout.

### 4.2 Change Impact Analysis
Prior to approving a pull request modifying existing baselines:
* The author must document affected requirements in the PR description.
* The Requirements Traceability Matrix (`docs/requirements/traceability_matrix.md`) must be updated concurrently with the code change.
* Reverification suites identified during impact analysis must be executed and confirmed passing.

### 4.3 Engineering Change Order (ECO) & Baseline Continuity
In compliance with DO-178C Section 7.2, any post-audit modification to baselined requirements, interfaces, or algorithmic constants mandates an Engineering Change Order (ECO).

To prevent specification drift between what was formally audited at time T and the evolving software:
* **Atomic Changeset Rule:** No code modification shall be merged without concurrently updating in the exact same commit:
  1. The governing requirement (`HLR.md` or low-level design in `docs/design/`).
  2. The Requirements Traceability Matrix (`traceability_matrix.md`).
  3. The document Revision History referencing the explicit `ECO-XXX` identifier.
* **Immutable Baseline Tagging:** Formal project baselines corresponding to SOI milestones are cryptographically locked using annotated Git tags (`v0.1.0-SOI-1`, `v0.2.0-SOI-2`).

---

## 5. Storage, Retrieval & Release Control

### 5.1 Repository Storage & Mirroring
* **Primary Repository:** Hosted under Git distributed architecture (`TomasUPV/attitude-estimator-cpp`).
* **Integrity Protection:** Cryptographic SHA-256 commit hashes ensure complete history immutability and tamper detection.

### 5.2 Release Media & Binary Artifacts
Official release baselines bundle:
1. Frozen source code tarball matching the release tag.
2. The complete documentation baseline (`docs/`).
3. CI verification execution logs and structural coverage reports.

---

## 6. Environment & Data Retention

* **Environment Reproducibility:** Exact compiler, linter, and build tool versions are cataloged in `docs/planning/SECI.md` to ensure identical binary output reconstruction at any future date.
* **Data Retention Duration:** In compliance with commercial aerospace operational guidelines, all baselined lifecycle data, test suites, and compiler flags shall remain retrievable throughout the active operational lifespan of the host aircraft.
