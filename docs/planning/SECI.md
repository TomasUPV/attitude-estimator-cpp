# Software Environment Configuration Index (SECI)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.15  
**Target Design Assurance Level:** DAL B  
**Baseline Version:** 1.0  
**Status:** Released / SOI-1 Baseline  

---

## 1. Scope & Objective

This Software Environment Configuration Index (SECI) establishes the frozen baseline of hardware architectures, operating systems, compilers, build systems, static analyzers, and test execution harnesses utilized across the Attitude Estimator development and verification lifecycles.

In compliance with DO-178C Section 11.15, this baseline ensures bit-level deterministic builds and reproducible verification execution environments over the active operational lifespan of the product.

---

## 2. Host & Target Hardware Environments

| Environment Profile | Architecture | Operating System | Purpose |
| :--- | :--- | :--- | :--- |
| **Local Development Station** | Apple Silicon (ARM64) / x86_64 | macOS 14+ / Ubuntu 22.04 LTS | Engineering authoring, local test execution, and static checks |
| **Continuous Integration (CI)** | x86_64 Virtualized Runner | Ubuntu 22.04 LTS (`ubuntu-latest`) | Automated headless build, static analysis, and regression tests |
| **Target Execution Profile** | ARM Cortex-M / POSIX 64-bit | Bare-Metal / RTOS Profile | Target runtime environment model |

---

## 3. Toolchain & Compiler Infrastructure

All compilation flags are strictly controlled via `CMakeLists.txt` and CI configuration files to enforce zero warnings as errors:

| Tool Category | Software Name | Executable / Package | Target Baseline Version | Governing Invocation / Flags |
| :--- | :--- | :--- | :--- | :--- |
| **Build Configuration** | CMake | `cmake` | 3.22.0+ (Minimum 3.16) | `-B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON` |
| **Primary Compiler** | Clang / LLVM | `clang++` | 14.0.0+ | `-std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror` |
| **Secondary Compiler** | GCC | `g++` | 11.4.0+ | Parity validation across POSIX targets |
| **Host Toolchain** | AppleClang | `clang++` | 15.0.0+ | Local development builds (macOS) |

---

## 4. Static Analysis & Safety Enforcers

| Analysis Tool | Target Standards | Baselined Version | Execution Command / Configuration |
| :--- | :--- | :--- | :--- |
| **Cppcheck** | MISRA / Determinism | 2.7.0+ | `--enable=warning,style,performance,portability --error-exitcode=1` |
| **Clang-Tidy** | CERT C++ / Core Guidelines | LLVM 14.0.0+ | Safety checks defined in `.clang-tidy` |

---

## 5. Test Harness & Framework Dependencies

| Framework / Dependency | Package Version | Source / Retrieval Mechanism | Hash / Tag Verification |
| :--- | :--- | :--- | :--- |
| **GoogleTest Suite** | Release 1.14.0 | FetchContent (Zip Archive) | Tag `v1.14.0` via `CMakeLists.txt` |
| **Git SCM** | 2.34.0+ | System Distribution | Monotonic SHA-256 Commit Tracking |

---

## 6. Environment Baseline Control

Any modification, version upgrade, or flag alteration affecting the tools listed in this index constitutes a major lifecycle environment change and requires:
1. An updated Problem Report / Change Request impact assessment.
2. Complete re-execution of all static analysis, unit test suites, and regression benchmarks.
3. Revision bump of this document under formal Configuration Management control.
