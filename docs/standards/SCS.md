# Software Code Standard (SCS)

**System:** Attitude & Heading Reference System (AHRS)  
**Software Component:** Attitude Estimator (EKF Pitch/Roll Core)  
**Governing Standard:** RTCA DO-178C / EUROCAE ED-12C Section 11.8  
**Target Design Assurance Level:** DAL B  
**Baseline Version:** 0.1 (Draft)  
**Status:** In Work / Draft  

---

## 1. Scope & Objective

This Software Code Standard (SCS) defines mandatory programming constraints, type definitions, and structural conventions for C++17 source code within the Attitude Estimator component.

In accordance with RTCA DO-178C Section 5.3 and Section 11.8, this standard ensures that all compiled source units are:
* Strictly deterministic with statically bounded memory and execution time.
* Free from undefined, unspecified, and implementation-defined runtime behaviors.
* Completely verified against static analysis safety profiles (MISRA C++ / SEI CERT).

---

## 2. Mandatory Types & Header Inclusions

### 2.1 Explicit-Width Primitive Types
Raw primitive types (`float`, `double`, `int`, `long`, `short`, `char`) are prohibited across all public interfaces and internal state definitions. Code shall exclusively utilize types from `include/attitude_estimator/types.h`:

| Alias Name | Underlying C++ Type | Standard Compliance Purpose |
| :--- | :--- | :--- |
| `attitude::float32_t` | `float` (IEEE-754 32-bit single) | Explicit floating-point precision |
| `attitude::float64_t` | `double` (IEEE-754 64-bit double) | State covariance & attitude estimation |
| `std::uint32_t` | Fixed 32-bit unsigned integer | Monotonic timestamps & loop counters |
| `std::size_t` | Standard unsigned size type | Array container indexing |

### 2.2 Header Guards
Non-standard preprocessor directives such as `#pragma once` are strictly prohibited. Every header file shall enforce standard `#ifndef`, `#define`, and `#endif` guard patterns reflecting the file path:

```cpp
#ifndef ATTITUDE_ESTIMATOR_MODULE_NAME_H
#define ATTITUDE_ESTIMATOR_MODULE_NAME_H

// Declarations...

#endif // ATTITUDE_ESTIMATOR_MODULE_NAME_H
```
## 3. Deterministic Safety & Memory Constraints

### 3.1 Heap Allocation Prohibition
* Dynamic runtime memory allocation (`malloc`, `calloc`, `realloc`, `free`, `new`, `delete`) is strictly forbidden across all functional execution paths.
* Dynamic STL containers (`std::vector`, `std::map`, `std::string`, `std::list`) are prohibited.
* Memory storage shall exclusively employ stack-allocated fixed-capacity containers (`std::array`) or direct struct aggregation.

### 3.2 Exception Handling Prohibition
* C++ exceptions (`throw`, `try`, `catch`) are disabled to ensure deterministic control flow and predictable execution timing.
* Functions that guarantee zero failure shall be explicitly qualified with `noexcept`.
* Interface errors shall be communicated exclusively via return flags, status enums, or sanitization fallbacks.

### 3.3 Class Design & Utility Constraints
* Static utility classes containing only pure mathematical functions (e.g., `QuaternionMath`) shall explicitly delete the default constructor to prevent instantiation:

class QuaternionMath {
public:
    QuaternionMath() = delete;
    // ...
};
## 4. Arithmetic Precision & Defensive Coding Rules

### 4.1 Floating-Point Zero Division Protection
Direct division by a variable floating-point denominator is prohibited without an epsilon threshold comparison. Every division must enforce an explicit defensive guard:

```cpp
constexpr float64_t kEpsilon = 1e-12;
if (std::abs(denominator) < kEpsilon) {
    // Sanitized deterministic fallback
}
```
### 4.2 Constant Correctness & Immutability
* Member functions that do not modify class state shall be marked `const`.
* Variables and function arguments not modified after initialization shall be qualified with `const` or `constexpr`.

### 4.3 Explicit Conversions
Implicit type narrowing or sign casting is strictly forbidden. Any necessary type conversions shall use explicit `static_cast<T>()`.

---

## 5. Tooling & Automated Linter Enforcement

Compliance with this standard is continuously monitored via GitHub Actions:
* **Compiler Flags:** `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror`.
* **Cppcheck:** Safety profile (`--enable=warning,style,performance,portability --error-exitcode=1`).
* **Clang-Tidy:** Safety profile defined in `.clang-tidy` (`bugprone-*`, `cert-*`, `cppcoreguidelines-*`, `readability-*`).

---

## 6. Software Code Review Checklist (Peer Inspection & SQA Gate)

Prior to merging implementation code into the protected `main` baseline, the reviewer and SQA auditor shall execute and sign this verification checklist:

| Item # | Verification Criteria | DO-178C Criteria Reference | Verification Method | Result (Pass / Fail / N/A) | Evidence / Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CHK-SCS-01** | **No Dynamic Allocation:** Is code completely free of `new`, `delete`, `malloc`, `free`, and dynamic containers? | Section 5.3.2.b | Static Analysis / Code Inspection | [ ] | |
| **CHK-SCS-02** | **Explicit Types:** Are all numeric variables defined using explicit types (`float64_t`, fixed integers)? | Section 5.3.1.a | Code Inspection | [ ] | |
| **CHK-SCS-03** | **Standard Header Guards:** Do all headers use standard `#ifndef` guards instead of `#pragma once`? | Section 5.3.1.b | Code Inspection | [ ] | |
| **CHK-SCS-04** | **Epsilon Division Protection:** Are all division operations protected by defensive threshold checks? | Section 5.3.2.f | Code Inspection / Unit Test | [ ] | |
| **CHK-SCS-05** | **Zero Compiler Warnings:** Does code compile with zero warnings under `-Werror` and `-Wconversion`? | Section 5.3.2.a | CI Automated Build Log | [ ] | |
| **CHK-SCS-06** | **Static Analysis Clean:** Does code pass `cppcheck` and `clang-tidy` safety checks with zero violations? | Section 5.3.2.a | CI Automated Linter Log | [ ] | |

### Checklist Sign-off Record
* **Target Source Units:** `src/*.cpp`, `include/attitude_estimator/*.h`
* **Commit Hash / Branch:** ____________________
* **Software Developer:** ______________________ | Date: ____________
* **Independent Code Reviewer:** ______________ | Date: ____________
* **SQA Gatekeeper Approval:** ________________ | Date: ___________
