# Design — SensorModel

Traces to: HLR-002, HLR-009

Data structures for IMU readings, plus (for the synthetic data
generator) a noise model that mimics a real MEMS accelerometer and
gyroscope. This module does NOT contain the filter itself — it only
defines the "shape" of sensor data flowing into it.

## Data structures

- `AccelReading`: 3-axis specific force, m/s², plus a timestamp
- `GyroReading`: 3-axis angular rate, rad/s, plus a timestamp
- `ImuSample`: pairs one `AccelReading` and one `GyroReading` at the
  same timestamp (assumes synchronized sampling for v1 — see note)

## Low-Level Requirements

| ID        | Requirement |
|-----------|-------------|
| LLR-SM-001 | `AccelReading` shall store 3-axis acceleration in m/s² and a timestamp in seconds. |
| LLR-SM-002 | `GyroReading` shall store 3-axis angular rate in rad/s and a timestamp in seconds. |
| LLR-SM-003 | `ImuSample` shall combine one `AccelReading` and one `GyroReading` sharing the same timestamp. |
| LLR-SM-004 | The synthetic data generator shall produce `ImuSample` sequences from a known ground-truth orientation trajectory, at a configurable sample rate (default 100 Hz, per HLR-002). |
| LLR-SM-005 | The synthetic data generator shall inject additive Gaussian noise on accelerometer and gyroscope readings, with configurable standard deviation per axis. |
| LLR-SM-006 | The synthetic data generator shall output the ground-truth orientation (quaternion) alongside each noisy `ImuSample`, for error computation against the filter's estimate. |

## Notes

- **Synchronized sampling assumption (v1)**: real IMUs often sample
  accelerometer and gyroscope at slightly different rates/times. For
  v1, this module assumes both arrive synchronized at the same
  timestamp — simplifies `EKF_Core`'s interface. If phase 2 moves to
  real hardware, this is the first assumption to revisit.
- **Noise model (LLR-SM-005)**: real MEMS noise isn't purely Gaussian
  (it also has bias drift, temperature dependence), but Gaussian
  white noise is the standard first-order approximation and is
  sufficient to validate the filter's core behavior. This is a
  deliberate, documented simplification — not an oversight.
- **LLR-SM-006 is what makes HLR-009 (verifiability) possible**:
  without ground truth attached to each sample, you can't measure
  error, only "the filter runs without crashing."
