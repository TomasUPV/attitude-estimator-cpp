# Plan for Software Aspects of Certification (PSAC)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Target Standard:** RTCA DO-178C / EUROCAE ED-12C  
**Baseline Target:** DAL B (with DAL C Standby Applicability)  
**Document Version:** 0.1 (Draft)  
**Status:** In Work / Draft  

---

## 1. System Overview

The Attitude & Heading Reference System (AHRS) provides spatial orientation data (pitch, roll, and derived yaw rates) to onboard flight deck instruments and flight management units. 

The software component governed under this plan executes the sensor fusion kernel, transforming high-frequency digitized tri-axial specific force ($a_x, a_y, a_z$) and tri-axial angular rates ($\omega_x, \omega_y, \omega_z$) from an inertial sensor block into dynamic pitch and roll orientation states.

System functional and architectural requirements are governed by parent system documents and allocated down to the software domain in accordance with RTCA DO-178C Section 2.

---

## 2. Software Overview

### 2.1 Functional Scope
The software package (`attitude_estimator`) is implemented in deterministic C++17. It filters sensor noise, corrects gyroscope drift using accelerometer gravity vectors, and yields spatial orientation represented internally as a normalized Hamilton unit quaternion.

The key functional requirements allocated to this software encompass:
* Monotonic timestamp ingestion and guardrail bounds ($0 < \Delta t \le 0.1\text{ s}$).
* Numerical state covariance integration and continuous Euclidean quaternion normalization.
* Deterministic innovation evaluation and Extended Kalman Filter (EKF) measurement correction.
* Transformation to Euler pitch and roll angles strictly for telemetry/display ingress.

### 2.2 Modular Partitioning
The software is partitioned into four decoupled modules:
1. **`QuaternionMath`**: Stateless Hamilton algebra, kinematic derivatives, Euclidean normalization, and conversion routines.
2. **`SensorModel`**: IMU data type definitions and synthetic deterministic trajectory generation.
3. **`EKF_Core`**: Discrete Kalman filter state propagation, Jacobian matrix computations, covariance updates, and measurement corrections.
4. **`AttitudeEstimator`**: Outer interface facade handling input validation, timestamp monotonicity, filter state management, and output conversion.

---

## 3. Certification Considerations

### 3.1 Failure Condition Allocation (ARP4761 FHA Reference)
In accordance with the Functional Hazard Assessment (`docs/planning/FHA_summary.md`), system-level hazards and their allocated Design Assurance Levels (DAL) are defined as follows:

| Hazard ID | Failure Condition Classification | Severity (ARP4761) | Assigned Software DAL |
| :--- | :--- | :--- | :--- |
| **FHA-AHRS-001** | Undetected Erroneous Attitude Output (Hazardously Misleading Information - HMI) | **Hazardous / Severe-Major** | **DAL B** |
| **FHA-AHRS-002** | Total Loss of Attitude Estimation Function (Loss of Function - LOF) | **Major** (Config A) / **Minor** (Config B) | **DAL C** |
| **FHA-AHRS-003** | Degraded Precision / Out-of-Tolerance Attitude Output | **Minor** | **DAL D** |

### 3.2 Target Software DAL Baseline
* **Primary Target (DAL B):** The software processes, life cycle data, verification activities, and coverage metrics are baseline-planned to meet **DO-178C DAL B** objectives (Annex A, Tables A-1 through A-10).
* **Dual-Applicability (DAL C):** If integrated within a secondary or cross-monitored standby instrument architecture (Configuration B), compliance evidence directly satisfies DAL C requirements without lifecycle alterations.

---

## 4. Software Lifecycle & Integral Processes

The software adheres to a classical V-model lifecycle incorporating continuous integral verification, configuration management, and quality assurance processes.

```text
  [ System Safety & FHA ]
             │
             ▼
  [ Planning & Standards (PSAC, SDP, SVP) ]
             │
             ▼
  [ High-Level Requirements (HLR) ] ◄────────► [ System / HLR Tests ]
             │                                         ▲
             ▼                                         │
  [ Software Architecture & LLRs ]  ◄────────► [ Unit Tests (GoogleTest) ]
             │                                         ▲
             ▼                                         │
  [ Deterministic Source Code (C++17) ] ───────────────┘
```

### 4.1 Development Processes
* **Requirements Process:** Capture of High-Level Requirements (`HLR.md`) derived from system performance and safety objectives.
* **Design Process:** Modular architecture specifications and Low-Level Requirements (`docs/design/*.md`) defining execution invariants, input/output data ranges, and memory layouts.
* **Implementation Process:** Source code implementation in C++17 adhering to safety-critical constraints (zero runtime dynamic memory, fixed-width types, deterministic execution paths).

### 4.2 Integral Processes
* **Verification Process:** Requirements-Based Testing (RBT) with GoogleTest, structural branch/decision coverage analysis (DAL B), and automated static code analysis (`cppcheck`, `clang-tidy`).
* **Configuration Management Process:** Git baseline tracking, strict semantic versioning, and problem reporting workflows governed by `docs/planning/SCMP.md`.
* **Quality Assurance Process:** In-process auditing and verification compliance monitoring governed by `docs/planning/SQAP.md`.

---

## 5. Software Lifecycle Data (Data Deliverables)

The following lifecycle data artifacts are generated and maintained under configuration control for certification liaison:

| Lifecycle Document / Data Item | File Path / Location | Baseline Target Milestone |
| :--- | :--- | :--- |
| Plan for Software Aspects of Certification (PSAC) | `docs/planning/PSAC.md` | SOI-1 Baseline |
| Software Development Plan (SDP) | `docs/planning/SDP.md` | SOI-1 Baseline |
| Software Verification Plan (SVP) | `docs/planning/SVP.md` | SOI-1 Baseline |
| Software Configuration Management Plan (SCMP) | `docs/planning/SCMP.md` | SOI-1 Baseline |
| Software Quality Assurance Plan (SQAP) | `docs/planning/SQAP.md` | SOI-1 Baseline |
| Software Requirements Standard (SRS) | `docs/standards/SRS.md` | SOI-1 Baseline |
| Software Design Standard (SDS) | `docs/standards/SDS.md` | SOI-1 Baseline |
| Software Code Standard (SCS) | `docs/standards/SCS.md` | SOI-1 Baseline |
| Software Environment Configuration Index (SECI) | `docs/planning/SECI.md` | SOI-1 Baseline |
| High-Level Requirements (HLR) | `docs/requirements/HLR.md` | SOI-2 Baseline |
| Software Design Description & LLRs | `docs/design/*.md` | SOI-2 Baseline |
| Requirements Traceability Matrix (RTM) | `docs/requirements/traceability_matrix.md` | SOI-2 / SOI-3 / SOI-4 |
| Source Code & Headers | `src/`, `include/` | SOI-3 Baseline |
| Test Cases, Procedures & Results | `tests/`, `docs/verification/` | SOI-3 Baseline |
| Software Accomplishment Summary (SAS) | `docs/verification/SAS.md` | SOI-4 Closeout |

---

## 6. Schedule & SOI Audit Milestones

Formal compliance reviews are aligned with the four Stages of Involvement (SOI) defined by civil aviation authorities (FAA Order 8110.49 / EASA CM-SWCEH-002):

[ Milestone ]       [ Scope & Primary Criteria ]
 SOI-1 (Planning)    Review and approval of PSAC, SDP, SVP, SCMP, SQAP, and Standards.
 SOI-2 (Req & Arch)  Audit of HLRs, LLRs, Architecture, and High-to-Low Traceability.
 SOI-3 (Code & Test) Audit of source code, static analysis, GTest execution, and Branch Coverage.
 SOI-4 (Final/SAS)   Audit of final binary, SAS closeout, closed RTM, and 0 Open Problem Reports.

 ## 7. Additional Considerations & Tooling (DO-330 Applicability)

### 7.1 Development & Build Toolchain
* **Compiler:** Clang 14+ / GCC 11+ / AppleClang 15+ enforcing strict safety flags: `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror`.
* **Build System:** CMake (minimum version 3.16).
* **Target Hardware Architecture:** 64-bit POSIX / ARM Cortex-M architecture compatibility.

### 7.2 Tool Qualification Considerations
In accordance with RTCA DO-178C Section 12.2 and RTCA DO-330 (Software Tool Qualification Considerations):
* **Static Analyzers (`cppcheck`, `clang-tidy`):** Used as automated assistance for developer review. Because static analysis findings are verified by independent developer peer reviews and confirmed via compiler diagnostic reports, these tools do not eliminate manual verification activities; therefore, tool qualification under DO-330 is not required.
* **Test Harness & Execution Framework (`GoogleTest`):** Test suites directly exercise compiled object code against documented requirements. Test execution results, logs, and structural decision coverage are independently verified; therefore, qualification of the test harness framework is not required.
* **Continuous Integration (`GitHub Actions`):** Automated build and test pipeline serves strictly for repeatable execution monitoring; all official baselines require verified manual audit logging.

---

## 8. Supplier Oversight

All software components, algorithmic models, test cases, and configuration lifecycle data are developed entirely in-house under this unified certification baseline. No external commercial-off-the-shelf (COTS) binary or third-party proprietary source software is integrated into the executable flight envelope.
