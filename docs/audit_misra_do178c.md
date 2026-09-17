# Auditoría de Cumplimiento: MISRA-like y DO-178C (Revisión 2)

**Proyecto:** `attitude-estimator-cpp`  
**Fecha de Revisión:** Septiembre 2026 (Segunda Pasada)  
**Alcance:** Todos los documentos, especificaciones, configuraciones y código fuente tras las últimas correcciones.

---

## 1. Resumen Ejecutivo de la Revisión 2

| Estándar | Estado Previo | Estado Actual | Variación / Progreso |
|---|---|---|---|
| **MISRA C++ / AUTOSAR** | 🔴 No conforme | 🟡 En camino (Cumplimiento en núcleo matemático) | **Gran avance.** Se han corregido las no conformidades críticas en cabeceras y fuentes implementados (`types.h`, guardas estándar, constructor borrado, `noexcept`, $\epsilon$ en floats). Resta extender el pipeline CI. |
| **DO-178C** | 🟡 Esqueleto Inicial | 🟡 Esqueleto Inicial | Sin cambios en la trazabilidad: persisten discrepancias en la matriz (falsos `Verified`) y falta cobertura estructural / pruebas de robustez. |

---

## 2. Novedades y Mejoras Implementadas

1. **Tipos con ancho y signo explícitos (AUTOSAR A3-9-1 / MISRA C++:2008 Regla 3-9-2):**
   - Creado [`include/attitude_estimator/types.h`](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/include/attitude_estimator/types.h) definiendo `float32_t` y `float64_t` en el namespace `attitude`.
   - Adoptado en [`quaternion_math.h`](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/include/attitude_estimator/quaternion_math.h) y [`quaternion_math.cpp`](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/src/quaternion_math.cpp).
2. **Guardas de preprocesador estándar (MISRA C++:2008 Regla 16-2-3 / MISRA C++:2023 Regla 19.3):**
   - Eliminado `#pragma once` en favor de `#ifndef ATTITUDE_ESTIMATOR_...`.
3. **Clases de utilidad estática (AUTOSAR A12-1-1 / MISRA C++:2008 Regla 12-1-1):**
   - Declarado constructor eliminado `QuaternionMath() = delete;` para evitar instanciaciones indebidas.
4. **Especificación `noexcept` (AUTOSAR A15-4-2 / MISRA C++:2023 Regla 15.4):**
   - Métodos `norm()` y `normalize()` anotados explícitamente como `noexcept`.
5. **Comparación segura de flotantes (MISRA C++:2008 Regla 6-2-2 / MISRA C++:2023 Regla 6.2.2):**
   - Sustituido el chequeo `n <= 0.0` por `constexpr float64_t kEpsilon = 1e-12; if (n < kEpsilon)`, protegiendo contra divisiones por cero o normas subnormales.
6. **Limpieza del árbol de ficheros:**
   - Eliminados los ficheros desubicados en la raíz (`attitude_estimator.h` y `attitude_estimator.cpp`).

---

## 3. Puntos Pendientes para Plena Conformidad

### 3.1. MISRA C++ / AUTOSAR
1. **Configuración de CI ([.github/workflows/ci.yml](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/.github/workflows/ci.yml)):**
   - `clang-tidy` sigue ejecutándose únicamente sobre `src/quaternion_math.cpp` (debe expandirse a todo `src/*.cpp`).
   - `cppcheck` suprime funciones no usadas y no invoca el módulo oficial `--addon=misra`.
2. **Archivos pendientes de implementación:**
   - `src/attitude_estimator.cpp`, `src/ekf_core.cpp` y `src/sensor_model.cpp` continúan con 0 bytes. Deberán mantener este mismo rigor de codificación cuando se implementen.

### 3.2. DO-178C
1. **Discrepancias en la Matriz de Trazabilidad ([docs/requirements/traceability_matrix.md](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/docs/requirements/traceability_matrix.md)):**
   - `LLR-QM-001` (`QuaternionMath::multiply`) figura como `Verified`, pero el método no existe aún en el código ni en los tests.
   - `LLR-EKF-009` figura como `Verified`, pero `ekf_core.cpp` está vacío.
   - *Acción requerida:* Marcar ambos como `Pending` para no incurrir en no conformidades de auditoría DO-178C (falsos positivos de verificación).
2. **Pruebas de Robustez (DO-178C Tabla A-7):**
   - [tests/test_quaternion_math.cpp](file:///Users/tomasherrerovalero/projects/attitude-estimator-cpp/tests/test_quaternion_math.cpp) sólo contiene 2 casos de prueba nominales.
   - *Acción requerida:* Añadir un caso de robustez para `Quaternion{0.0, 0.0, 0.0, 0.0}` o normas $< 10^{-12}$ que verifique que `normalize()` devuelve efectivamente el cuaternión identidad sin producir `NaN` o excepciones.
3. **Cobertura de Código Estructural (DO-178C Tabla A-7):**
   - Sigue pendiente la activación de instrumentación de cobertura (`gcov` / `lcov` / `llvm-cov`) en CMake.
