# Software Verification Plan (SVP)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.3  
**Target Design Assurance Level:** DAL B  
**Document Version:** 1.0  
**Status:** Released / SOI-1 Baseline  

---

## 1. Verification Organization & Independence

In accordance with RTCA DO-178C Table A-3 through Table A-7, verification activities enforce organizational and technical independence:

* **Independent Verification Responsibility:** Test procedures, verification test cases, and structural coverage assessments shall be developed and reviewed by workflows and roles distinct from the primary author of the implementation code.
* **Role Separation Protocol:**
  * **Development Role:** Responsible for `HLR.md`, module architecture, and `src/` implementation.
  * **Verification Role:** Responsible for `tests/` suites, boundary test injection, linter enforcement, and coverage logs.
  * Formal peer review checklists gate transitions between branches to confirm independent verification sign-off.

### 1.1 Independence Mitigation for Solo Development
In full compliance with RTCA DO-178C Table A-5/A-6 expectations for DAL B software, verification activities require independence between the development and test authoring processes. In this single-engineer simulated environment, cognitive bias is mitigated through formal decoupling mechanisms:

* **Requirements-First Adversarial Test Authoring:** Negative, robustness, and boundary test cases are authored and committed prior to functional implementation. Tests are designed with an adversarial posture specifically aimed at breaking algorithms, mathematical normalization, and time monotonicity.
* **Decoupled Verification Agent Reviews:** Independent automated review passes are executed using isolated prompt/evaluation instances without access to implementation context, acting strictly as external V&V auditors tasked with finding edge-case oversights.
* **Automated CI Gating:** Static analysis (`cppcheck`, `clang-tidy` with safety profile) and compiler warning gates (`-Werror`) operate as impartial verification arbiters, blocking commits with unhandled boundary conditions or suppressed checks.

---

## 2. Verification Methods

Verification of the Attitude Estimator encompasses three complementary activities: Reviews, Static Analysis, and Requirements-Based Testing (RBT).

```text
  [ High-Level Requirements ] ────────► Review / Trace Analysis
              │
              ▼
  [ Low-Level Requirements  ] ────────► Review / Trace Analysis
              │
              ▼
  [ Source Code (C++17)     ] ────────► Static Analysis (Linters & Compilers)
              │
              ▼
  [ Executable Target Object] ────────► Requirements-Based Testing (GoogleTest)
  ```
### 2.1 Reviews & Inspections
* **Requirements Review:** Inspection of HLRs and LLRs against `docs/standards/SRS.md` verifying accuracy, determinism, testability, and absence of ambiguity.
* **Design & Architecture Review:** Inspection of module interfaces, matrix dimension guarantees, and numerical guardrails against `docs/standards/SDS.md`.
* **Code Peer Review:** Verification that source code strictly complies with `docs/standards/SCS.md` (MISRA/AUTOSAR subset, zero dynamic memory allocation, fixed-width types).

### 2.2 Static Code Analysis
Automated static checks are executed within the build pipeline:
* **Compiler Diagnostics:** Strict zero-warning build policy (`-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror`).
* **Cppcheck:** Safety-critical profile checking for out-of-bounds array access, uninitialized variables, and arithmetic exceptions.
* **Clang-Tidy:** Enforcing SEI CERT C++, bugprone patterns, and modernization constraints forbidding unsafe runtime operations.

### 2.3 Requirements-Based Testing (RBT)
Dynamic verification exercises the compiled machine code strictly against allocated requirements:
1. **Low-Level Tests (LLT - Unit Testing):** GoogleTest test fixtures mapping 1:1 with Low-Level Requirements in `docs/design/*.md`.
   * **Normal Range Testing:** Validating standard convergence, quaternion multiplication, and matrix propagation.
   * **Robustness & Boundary Testing:** Injecting abnormal inputs including dt <= 0, extreme angular rates, sensor saturation, and zero-norm vectors.
   * **Adversarial Robustness Test Cases:**
     * **Mathematical Edge Cases:** Ingestion of null vectors ($\mathbf{a} = [0, 0, 0]^T$), degenerate quaternions, and non-orthogonal state vectors.
     * **Temporal Integrity Violations:** Ingestion of negative intervals ($dt \le 0$) and stall conditions ($dt > 0.1\text{ s}$).
     * **Numerical Bounds:** Verification that float comparisons employ strict epsilon margins ($\epsilon = 10^{-12}$) to prevent subnormal division or NaN propagation.
2. **High-Level Tests (HLT - Integration Testing):** Validating end-to-end filter convergence against synthetic noisy 6-DOF IMU profiles, verifying that estimated Euler pitch/roll errors remain within the 1.5° envelope.

---

## 3. Verification Environment & Test Harness

Verification execution is tied to the frozen toolchain defined in `docs/planning/SECI.md`:
* **Host Test Environment:** x86_64 / ARM64 POSIX execution framework.
* **Unit Test Framework:** GoogleTest (v1.12.0+) driven via CMake `ctest`.
* **Continuous Integration:** GitHub Actions executing the complete test suite on every pull request, producing immutable pass/fail artifacts and test logs.
* **Target Environment Equivalence:** Compilation flags, floating-point math libraries, and compiler optimizations used in unit tests mirror the target configuration to eliminate behavioral discrepancies.

---

## 4. Structural Coverage Analysis (DAL B Objectives)

In accordance with DO-178C Table A-7 (Objective 3 and Objective 4):
* **DAL B Coverage Criteria:** Verification activities must achieve 100% **Statement Coverage** and 100% **Decision Coverage (Branch Coverage)** across all non-dead executable object code.
* **Coverage Measurement:** Generated using GCC/Clang profiling options (`-fprofile-arcs -ftest-coverage` or `llvm-cov`) and reviewed using `gcov`/`lcov` reporting.
* **Coverage Analysis Protocol:**
  * If a decision path is unexecuted, an analysis shall determine whether the omission represents insufficient test cases (RBT defect) or undocumented software behavior (dead/extraneous code).
  * Missing coverage due to defensive safety checks (dt <= 0 guards, normalization epsilon thresholds) must be covered by dedicated robustness test procedures.

---

## 5. Transition Criteria & Reverification

### 5.1 Verification Transition Criteria
* Verification commences only after relevant HLRs and LLRs have been reviewed, baselined, and linked in `docs/requirements/traceability_matrix.md`.
* Source code must pass static analysis clean of warnings before official test case verification runs are recorded.

### 5.2 Reverification Method (Regression Analysis)
Any modification to requirements, design, or source code triggers the following reverification steps:
1. **Traceability Impact Analysis:** The developer identifies all upstream requirements, peer modules, and downstream test procedures impacted by the change.
2. **Targeted Reverification:** Re-execution of the specific test cases linked to the modified LLRs.
3. **Full Regression Suite:** Automated re-execution of the complete test harness via CI to ensure zero regression across unchanged functional paths.

---

## 6. Additional Verification Considerations

### 6.1 Partitioning & Memory Integrity
The attitude estimator operates with zero dynamic heap allocation (`std::array` only), executing within fixed, statically predictable stack frames. Verification tests confirm that no pointer arithmetic breaches module memory bounds.

### 6.2 Compiler Assumptions
The test pipeline assumes deterministic IEEE-754 double-precision floating-point compliance (`float64_t`). Compiler flags forbidding unsafe floating-point optimizations (such as disabling `-ffast-math`) are strictly enforced.

### 6.3 Previously Developed Software (PDS) & Dissimilar Software
No previously developed software (PDS) or commercial off-the-shelf (COTS) binary routines are incorporated into the algorithmic core. All functional code is developed and verified natively under this plan.
