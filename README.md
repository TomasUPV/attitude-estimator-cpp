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

Before developing requirements or code, DO-178C requires establishing and approving the lifecycle governance plans:

* **Stage Target**: SOI-1 Review (Audit of software plans and standards).
* **Baseline Artifacts**:
  * [`docs/planning/PSAC.md`](docs/planning/PSAC.md): Plan for Software Aspects of Certification.
  * [`docs/planning/SDP.md`](docs/planning/SDP.md): Software Development Plan.
  * [`docs/planning/SVP.md`](docs/planning/SVP.md): Software Verification Plan.

---

## 3. Architecture Overview

The software is structured into four deterministic modules:
* **`QuaternionMath`**: Pure, stateless Hamilton quaternion operations with epsilon safeguards against zero division.
* **`SensorModel`**: Sensor reading data structures and synthetic trajectory generation with Gaussian noise injection.
* **`EKF_Core`**: State propagation, Jacobian evaluation, and Kalman measurement update.
* **`AttitudeEstimator`**: System facade handling sample validity and timestamp monotonicity checks.
