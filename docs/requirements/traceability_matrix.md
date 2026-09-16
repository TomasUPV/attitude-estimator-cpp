# Requirements Traceability Matrix (RTM)

This matrix establishes end-to-end bidirectional traceability between High-Level Requirements (HLR), Low-Level Requirements (LLR), source implementation units, and verification tests.

## Status Legend
- **Implemented**: Source code written and passing static analysis.
- **Verified**: Automated unit/integration test implemented and passing.
- **Pending**: Artifact planned but awaiting implementation.

---

## Traceability Table

| HLR ID | LLR ID | Module / Function | Verification Test | Status |
|---|---|---|---|---|
| **HLR-001** | LLR-QM-001 | `QuaternionMath::multiply` | `QuaternionMathTest.MultiplicationIdentity` | Verified |
| | LLR-QM-002 | `QuaternionMath::normalize` | `QuaternionMathTest.NormalizationProducesUnitLength` | Verified |
| | LLR-QM-003 | `QuaternionMath::norm` | `QuaternionMathTest.DefaultQuaternionHasUnitNorm` | Verified |
| | LLR-QM-004 | `QuaternionMath::toEulerAngles` | `QuaternionMathTest.EulerAngleConversionZeroRotation` | Pending |
| | LLR-AE-005 | `AttitudeEstimator::getOrientationQuaternion` | `AttitudeEstimatorTest.ReturnsNormalizedOrientation` | Pending |
| | LLR-AE-006 | `AttitudeEstimator::getEulerAngles` | `AttitudeEstimatorTest.ConvertsToEuler` | Pending |
| **HLR-002** | LLR-SM-001 | `SensorModel::AccelReading` | `SensorModelTest.DataIntegrity` | Pending |
| | LLR-SM-002 | `SensorModel::GyroReading` | `SensorModelTest.DataIntegrity` | Pending |
| | LLR-SM-003 | `SensorModel::ImuSample` | `SensorModelTest.DataIntegrity` | Pending |
| | LLR-AE-002 | `AttitudeEstimator::processSample` | `AttitudeEstimatorTest.ValidatesSampleMonotonicity` | Pending |
| **HLR-003** | LLR-EKF-001 | `EKF_Core::initialize` | `EKFCoreTest.InitializesUnitNormState` | Pending |
| | LLR-EKF-002 | `EKF_Core::predict` | `EKFCoreTest.PropagatesStateWithExplicitDt` | Pending |
| | LLR-EKF-003 | `EKF_Core::predict` | `EKFCoreTest.PropagatesCovarianceP` | Pending |
| | LLR-AE-003 | `AttitudeEstimator::processSample` | `AttitudeEstimatorTest.ComputesExplicitDeltaTime` | Pending |
| **HLR-004** | LLR-EKF-004 | `EKF_Core::update` | `EKFCoreTest.NormalizesAccelerometerMeasurement` | Pending |
| | LLR-EKF-005 | `EKF_Core::update` | `EKFCoreTest.ComputesValidKalmanGain` | Pending |
| | LLR-EKF-006 | `EKF_Core::update` | `EKFCoreTest.UpdatesStateAndCovariance` | Pending |
| | LLR-AE-004 | `AttitudeEstimator::processSample` | `AttitudeEstimatorTest.DispatchesUpdateStep` | Pending |
| **HLR-005** | LLR-QM-005 | `QuaternionMath::derivative` | `QuaternionMathTest.DerivativeZeroAngularVelocity` | Pending |
| | LLR-EKF-007 | `EKF_Core::normalizeState` | `EKFCoreTest.MaintainsUnitNormPostStep` | Pending |
| **HLR-006** | LLR-EKF-009 | `EKF_Core` (Static Allocation) | Structural Code Review / Linter Checks | Verified |
| **HLR-007** | LLR-AE-001 | `AttitudeEstimator::initialize` | `AttitudeEstimatorTest.DefaultInitialization` | Pending |
| | LLR-AE-007 | `AttitudeEstimator::processSample` | `AttitudeEstimatorTest.DiscardsInvalidTimestamps` | Pending |
| **HLR-008** | LLR-EKF-008 | `EKF_Core::getState` | `EKFCoreTest.ExposesImmutableState` | Pending |
| **HLR-009** | LLR-SM-004 | `SyntheticDataGenerator::generate` | `DataGeneratorTest.GeneratesConfigurableTrajectory` | Pending |
| | LLR-SM-005 | `SyntheticDataGenerator::injectNoise` | `DataGeneratorTest.AppliesGaussianNoiseDistribution` | Pending |
| | LLR-SM-006 | `SyntheticDataGenerator::groundTruth` | `DataGeneratorTest.OutputsGroundTruthWithSample` | Pending |
