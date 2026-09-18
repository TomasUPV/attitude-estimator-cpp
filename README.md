# Attitude Estimator — Full DO-178C Lifecycle Simulation

[![Continuous Integration](https://github.com/TomasUPV/attitude-estimator-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/TomasUPV/attitude-estimator-cpp/actions/workflows/ci.yml)
[![Standard: C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Compliance: MISRA--like](https://img.shields.io/badge/Compliance-MISRA--like%20%2F%20CERT-brightgreen.svg)]()
[![Process: DO--178C](https://img.shields.io/badge/Process-DO--178C%20Simulated-orange.svg)]()

This repository implements a 6-DOF IMU pitch and roll attitude estimator using an Extended Kalman Filter (EKF), simulating the complete verification and certification lifecycle mandated by **RTCA DO-178C** (Design Assurance Level B/C).

> **Aviation Certification Framework:**  
> This project demonstrates the step-by-step engineering processes required to achieve flight authorization, progressing sequentially through each Stage of Involvement (SOI) audit.

---

## 1. Problem & Performance Targets

Estimating orientation from low-cost MEMS sensors requires combining rate gyroscopes (subject to drift) with accelerometers (subject to vibration and dynamic acceleration).

* **Steady-State Error**: Less than 1.5 degrees in pitch and roll under nominal static conditions.
* **Dynamic Recovery**: Convergence from an orientation error greater than 30 degrees within 3.0 seconds.
* **Numerical Stability**: Zero filter divergence over a continuous 10-minute simulated trajectory.

*(Initial targets derived from typical MEMS noise characteristics; validated empirically against synthetic ground-truth datasets).*

---

## 2. Current Lifecycle Stage: Phase 1 — Planning (SOI-1)

Before developing requirements or code, RTCA DO-178C / EUROCAE ED-12C mandates establishing, baselining, and approving the lifecycle governance plans, technical standards, and tool qualification assumptions:

* **Stage Target**: Stage of Involvement 1 (SOI-1) Review — Software Planning and Standards Approval.
* **Stage Status**: **SIMULATION PASSED / DRY-RUN GATE CLEARED** (Internal baseline verification completed without open non-conformances).

### Baseline Lifecycle Artifacts (v1.0 Frozen)

* **System Safety & Criticality Allocation**:
  * [`docs/planning/FHA_summary.md`](docs/planning/FHA_summary.md): Functional Hazard Assessment (ARP4761) allocating **DAL B** to prevent Hazardously Misleading Information (HMI).
* **Regulatory Contract**:
  * [`docs/planning/PSAC.md`](docs/planning/PSAC.md): Plan for Software Aspects of Certification establishing system architecture, compliance matrices, and DO-178C Table A-1 objectives.
* **Core Lifecycle Management Plans**:
  * [`docs/planning/SDP.md`](docs/planning/SDP.md): Software Development Plan (V-model, deterministic C++17 design rules, static footprint).
  * [`docs/planning/SVP.md`](docs/planning/SVP.md): Software Verification Plan (Requirements-Based Testing via GoogleTest, static analysis linters, Decision Coverage target).
  * [`docs/planning/SCMP.md`](docs/planning/SCMP.md): Software Configuration Management Plan (Git branching model, change control, cryptographic release tagging).
  * [`docs/planning/SQAP.md`](docs/planning/SQAP.md): Software Quality Assurance Plan (Independent peer reviews, conformity audits, process integrity).
* **Engineering Standards**:
  * [`docs/standards/SRS.md`](docs/standards/SRS.md): Software Requirements Standard (Syntax guidelines, verifiability criteria, no implementation coupling).
  * [`docs/standards/SDS.md`](docs/standards/SDS.md): Software Design Standard (Modular partitioning, memory invariants, deterministic execution bounds).
  * [`docs/standards/SCS.md`](docs/standards/SCS.md): Software Code Standard (MISRA/CERT-aligned C++17 rules: no `malloc`/`new`, fixed-width types `float64_t`).
* **Environment Control**:
  * [`docs/planning/SECI.md`](docs/planning/SECI.md): Software Environment Configuration Index (Deterministic build/test toolchain versions frozen).

### SOI-1 Dry-Run & Process Verification Audit

A simulated compliance dry run was executed against the **DO-178C Table A-1** lifecycle planning objectives to authorize progression into software development:

* [`doc:s/planning/SOI_1_checklist.md`](docs/planning/SOI_1_checklist.md): Formal compliance audit gate verifying document baselines, DAL allocation integrity, toolchain reproducibility, and transition criteria. All verification checklist points marked as satisfied (`PASS`).

<div align="center">

<img src="images/soi_1_workflow.png" alt="DO-178C Phase 1 Lifecycle Workflow" width="100%" />

<p><em>Figure 1: DO-178C Phase 1 (SOI-1) Lifecycle Baseline & Audit Progression Pipeline.</em></p>

</div>
## 3. Architecture Overview

The software is structured into four deterministic modules:
* **`QuaternionMath`**: Pure, stateless Hamilton quaternion operations with epsilon safeguards against zero division.
* **`SensorModel`**: Sensor reading data structures and synthetic trajectory generation with Gaussian noise injection.
* **`EKF_Core`**: State propagation, Jacobian evaluation, and Kalman measurement update.
* **`AttitudeEstimator`**: System facade handling sample validity and timestamp monotonicity checks.
