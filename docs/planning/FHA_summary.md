# Functional Hazard Assessment (FHA) Summary

**System Item:** Attitude & Heading Reference System (AHRS) — Attitude Estimator Software Function  
**Governing Standard:** ARP4761 / ARP4754A / RTCA DO-178C Section 2  
**Baseline Version:** 1.0  
**Status:** Released / SOI-1 Baseline  

---

## 1. Scope & Objective

This document summarizes the system-level safety analysis allocated to the Attitude Estimator software component within the onboard avionics architecture. 

In accordance with **ARP4761 (Safety Assessment Process)** and **ARP4754A**, the objective of this Functional Hazard Assessment (FHA) is to:
1. Identify potential failure conditions resulting from anomalies within the pitch and roll estimation function.
2. Classify the severity of each failure condition according to its operational impact on the aircraft and flight crew.
3. Establish the required **Design Assurance Level (DAL)** for the software lifecycle processes governed under **RTCA DO-178C / EUROCAE ED-12C**.

---

## 2. System Operational Context & Architecture Partitioning

The software function under evaluation processes digitized tri-axial specific force and tri-axial angular rate from a 6-DOF Inertial Measurement Unit (IMU) to produce dynamic pitch and roll state estimates via an Extended Kalman Filter (EKF).

The host avionics environment dictates the operational criticality:
* **Configuration A (Primary Flight Instrument / Autopilot Ingress):** The estimator directly feeds the Primary Flight Display (PFD) and flight control logic in Instrument Meteorological Conditions (IMC).
* **Configuration B (Secondary / Standby Attitude Reference):** The estimator acts as an independent backup channel cross-monitored against a certified primary IRS/AHRS, requiring manual crew action or majority-voting disengagement upon anomaly detection.

---

## 3. Failure Condition Classification Matrix (ARP4761)

| Failure ID | Failure Mode Description | Crew & Flight Phase Impact | Severity Classification (ARP4761) | Software DAL Allocation (DO-178C) |
| :--- | :--- | :--- | :--- | :--- |
| **FHA-AHRS-001** | **Undetected Erroneous Pitch/Roll Output (Hazardously Misleading Information - HMI)**<br>Filter outputs smooth, divergent, or frozen attitude data without triggering an integrity/invalidity flag during IMC flight. | Crew unaware of false spatial attitude; risk of spatial disorientation, uncommanded attitude deviation, or inappropriate flight control inputs. Significant reduction in safety margins. | **Hazardous / Severe-Major** *(Configuration A)*<br>(Target: 1e-7 / flight hour) | **DAL B** |
| **FHA-AHRS-002** | **Total Loss of Attitude Estimation Function (Loss of Function - LOF)**<br>Estimator enters an unrecoverable fault state, crashes, halts sample processing, or asserts an explicit invalidity flag. | Immediate degradation of automated flight control; crew must revert to secondary or standby attitude instrumentation. Workload increases significantly, but safe flight is maintained. | **Major** *(Configuration A)* / **Minor** *(Configuration B)*<br>(Target: 1e-5 / flight hour) | **DAL C** |
| **FHA-AHRS-003** | **Degraded Precision / Out-of-Tolerance Attitude Estimates**<br>Steady-state error exceeds 1.5° or recovery time exceeds 3.0 s without divergence. | Degraded control smoothness and increased pilot workload during manual approach; no immediate hazard to airframe integrity. | **Minor**<br>(Target: 1e-3 / flight hour) | **DAL D** |

---

## 4. Software DAL Justification & Baseline Determination

Based on the failure mode analysis:

1. **Worst-Case Hazard Allocation (DAL B):**
   * Failure Mode **FHA-AHRS-001 (Hazardously Misleading Information)** carries a severity classification of **Hazardous** when deployed in a primary flight guidance role (Configuration A). 
   * Under DO-178C guidelines, software whose anomalous behavior could result in a Hazardous condition requires **Software Design Assurance Level B**.

2. **Dual-Certification Scope (DAL B / DAL C Dual Strategy):**
   * For the purpose of this software lifecycle and subsequent Stage of Involvement (SOI) audits, the software processes, design standards, and verification activities are planned to satisfy **DAL B** objectives:
     * Full bidirectional requirement traceability (HLR <-> LLR <-> Code <-> Test).
     * **Decision Coverage (Branch Coverage)** structural analysis.
     * Software independence across verification and quality assurance processes.
   * If integrated strictly as a standby instrument (Configuration B), the baseline can be transitioned to **DAL C** without architectural modification, relying on Statement Coverage verification.

---

## 5. Derived Safety Requirements Allocated to Software

To mitigate the risk of HMI (**FHA-AHRS-001**), the following top-level safety constraints are allocated to the software development lifecycle:

* **SR-SAF-001 (Monotonicity & Guardrails):** The software shall discard and flag samples with non-positive or excessive time deltas (dt <= 0 or dt > 0.1 s) to prevent numerical divergence of the kinematic integrator.
* **SR-SAF-002 (Unit Norm Preservation):** The state quaternion shall undergo deterministic Euclidean normalization after every prediction and update cycle to prevent arithmetic drift from physical reality.
* **SR-SAF-003 (Deterministic Execution):** The software shall avoid dynamic heap allocation (`malloc`, `new`) during runtime execution, ensuring fixed execution time bounds (WCET predictability) and zero risk of heap exhaustion crashes.

---

## 6. Baseline Reference

* **Allocated Parent Plan:** Plan for Software Aspects of Certification (`PSAC.md`, Section 1.3).
* **Downstream Artifact:** High-Level Requirements (`HLR.md`).
