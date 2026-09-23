# Software Development Plan (SDP)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.2  
**Target Design Assurance Level:** DAL B  
**Document Version:** 1.0  
**Status:** Released / SOI-1 Baseline  

---

## 1. Overview & Objectives

This Software Development Plan (SDP) defines the engineering lifecycle, development processes, transition criteria, and environment controls used to implement the Attitude Estimator software component.

The primary objective is to guarantee deterministic execution, mathematical correctness, and complete traceability from input system constraints down to the source code baseline, fulfilling all DO-178C Table A-3, A-4, and A-5 objectives.

---

## 2. Applicable Standards & Relationship to Lifecycle Data

The development activities defined herein shall strictly comply with the following baselined standards:
* **Software Requirements Standard (SRS):** `docs/standards/SRS.md` defines the syntax, verification criteria, and schema for High-Level Requirements (HLRs) and Low-Level Requirements (LLRs).
* **Software Design Standard (SDS):** `docs/standards/SDS.md` enforces modular decoupling, stateless algorithmic kernels, memory layout constraints, and fixed bounds.
* **Software Code Standard (SCS):** `docs/standards/SCS.md` dictates the C++17 safety profile, zero heap allocations, explicit fixed-width numeric types, and MISRA/AUTOSAR subset compliance.

---

## 3. Software Life Cycle Model

The project follows a classical **V-Model Development Lifecycle** composed of sequential engineering phases executed alongside continuous integral processes (Verification, Configuration Management, and Quality Assurance):
```text
 [ Planning Phase ] ──► [ Requirements Phase ] ──► [ Architecture & Design ]
                                                          │
                                                          ▼
 [ Target Integration ] ◄── [ Unit & Int Testing ] ◄── [ Implementation ]
 ```
 ### 3.1 Software Requirements Process
* **Inputs:** Functional Hazard Assessment Summary (`docs/planning/FHA_summary.md`), System Requirements (`docs/requirements/SR.md`), and Software Requirements Standard (`docs/standards/SRS.md`).
* **Activities:** 
  * Definition of operational, performance, timing, memory, and failure-detection High-Level Requirements (`docs/requirements/HLR.md`).
  * Explicit identification of derived software requirements and notification to system safety engineering.
* **Outputs:** Baselined `HLR.md` and forward-traceable elements in the Requirements Traceability Matrix (`docs/requirements/traceability_matrix.md`).

### 3.2 Software Design Process
* **Inputs:** Approved High-Level Requirements (`HLR.md`) and Software Design Standard (`docs/standards/SDS.md`).
* **Activities:**
  * Architectural decomposition into modular, decoupled units (`QuaternionMath`, `EKF_Core`, `SensorModel`, `AttitudeEstimator`).
  * Specification of Low-Level Requirements (LLRs) detailing operational interfaces, data representations, invariant execution bounds, and memory layout (`docs/design/*.md`).
* **Outputs:** Software Architecture Description and Low-Level Requirements baseline.

### 3.3 Software Coding Process
* **Inputs:** Approved Low-Level Requirements, Software Architecture, and Software Code Standard (`docs/standards/SCS.md`).
* **Activities:**
  * Deterministic C++17 implementation strictly adhering to safety rules: zero runtime dynamic memory allocation (`malloc`, `free`, `new`, `delete`), explicit fixed-width numeric types, and defensive invariant checks.
  * Continuous local static analysis verification prior to repository submission.
* **Outputs:** Deterministic C++ header files (`include/`) and source implementation files (`src/`).

### 3.4 Integration Process
* **Activities:** Stepwise linking of modular libraries (`libattitude_estimator.a`) and compilation of test harnesses managed via reproducible CMake targets.
* **Outputs:** Executable test suites, static analysis targets, and linked library binaries.

---

## 4. Software Development Environment

The software lifecycle environment is formally indexed and baselined under `docs/planning/SECI.md`:
* **Target Architecture:** 64-bit POSIX / Embedded ARM Cortex-M bare-metal & RTOS profile.
* **Build System:** CMake (minimum version 3.16) utilizing deterministic build trees and isolated build configurations.
* **Compiler Suites:** Clang 14+ / GCC 11+ / AppleClang 15+ configured with strict zero-tolerance safety flags:
  `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror`
* **Static Analysis Suites:** `cppcheck` (safety-critical profile) and `clang-tidy` (enforcing bugprone, cert, and modernize checks).

---

## 5. Formal Transition Criteria

In accordance with RTCA DO-178C Section 4.4 and Table A-3, phase-to-phase transitions are governed by explicit entry and exit gates:

| Lifecycle Transition | Entry Criteria | Exit / Transition Criteria |
| :--- | :--- | :--- |
| **Planning ➔ Requirements** | System safety constraints and FHA established; PSAC, plans, and standards baselined internally (Submitted for SOI-1). | High-Level Requirements authored, peer-reviewed, verified against SRS rules, and baselined under CM. |
| **Requirements ➔ Design** | Baselined HLRs; 100% forward traceability from System Requirements; zero open review defects. | Software Architecture defined; LLRs authored, peer-reviewed, and verified to trace 100% to HLRs (Prepared for SOI-2). |
| **Design ➔ Coding** | LLRs and Architecture baselined under Git; interfaces and numeric invariants approved. | Source code implementation complete; zero compiler warnings (`-Werror`); static analysis checks passed. |
| **Coding ➔ Verification** | Source code baselined under Git tag; build tree compiles deterministically. | 100% Requirements-Based Tests pass; Decision Coverage metrics met; Traceability Matrix closed (Prepared for SOI-3/4). |
