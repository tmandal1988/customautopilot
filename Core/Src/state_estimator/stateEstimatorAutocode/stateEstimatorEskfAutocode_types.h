//
// File: stateEstimatorEskfAutocode_types.h
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 7.0
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 23 09:38:38 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#ifndef stateEstimatorEskfAutocode_types_h_
#define stateEstimatorEskfAutocode_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#ifndef DEFINED_TYPEDEF_FOR_busImuData_
#define DEFINED_TYPEDEF_FOR_busImuData_

// Bus containing accelerometer and gyro data in body axis
struct busImuData
{
  // Acceleration in body axis including gravity
  real32_T bodyAccels_mps2[3];

  // Body angular rates from gyro
  real32_T bodyRates_radps[3];

  // Delta time between IMU readings
  real32_T dtImuTime_s;

  // Flag to indicate if Imu data is valid or not
  boolean_T isImuDataValid;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busMagData_
#define DEFINED_TYPEDEF_FOR_busMagData_

// 3-vector magnetic field in body axis as measured by a magnetometer strapped to the vehicle 
struct busMagData
{
  // Earth magnetic field in body axis
  real32_T bodyMagVector_uT[3];

  // true -> Mag data is valid
  // false -> Mag data in invalid
  boolean_T isMagDataValid;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busGpsData_
#define DEFINED_TYPEDEF_FOR_busGpsData_

// Bus containing raw GPS sensor data
struct busGpsData
{
  // Lat [deg], Lon [deg], Alt [m] from GPS
  real_T latLonAlt[3];

  // NED velocity from GPS
  real32_T nedVel_mps[3];

  // true -> GPS data is valid
  // false -> GPS data is invalid
  boolean_T isGpsDataValid;
  boolean_T isGpsInitialized;

  // GPS HPOS Accuracy
  real32_T hacc_m;

  // GPS VPOS Accuracy
  real32_T vacc_m;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busBaroData_
#define DEFINED_TYPEDEF_FOR_busBaroData_

// Baro data
struct busBaroData
{
  // Pressure measurement from the barometer
  real32_T pressure_pa;

  // Temperature data from barometer
  real32_T temp_c;

  // true -> baro data is valid
  // false -> baro data is invalid
  boolean_T isBaroDataValid;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busLidarData_
#define DEFINED_TYPEDEF_FOR_busLidarData_

// Bus contaning lidar range data
struct busLidarData
{
  // Range reported by the Lidar
  real32_T range_m;

  // true -> Lidar data is valid
  // false -> Lidar data is invalid
  boolean_T isLidarDataValid;

  // true -> Lidar initialized
  // false -> Lidar not initialized
  boolean_T isLidarInitialized;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busMtf01pData_
#define DEFINED_TYPEDEF_FOR_busMtf01pData_

// MTF01P optical flow data which include a a laser rangefinder and flow data
struct busMtf01pData
{
  // Raw range value from laser rangefinder in MTF01P data
  real32_T dist_m;

  // Distance value precision, lower is better
  uint8_T distPrecision;

  // Distance measrement strength, higher is better
  uint8_T distStrength;

  // 0 is invalid, 1 is valid
  uint8_T distStatus;

  // Flow value in sensor X direction
  real32_T flowX_radps;

  // Flow value in sensor Y direction
  real32_T flowY_radps;

  // Flow measurement status, 1 is valid and 0 is invalid
  uint8_T flowStatus;

  // Flow measurement quality
  uint8_T flowQuality;

  // A flag that indicates if we received a new MTF01P measurement
  boolean_T isMtf01pDataValid;

  // Time when the data was published (not captured)
  uint64m_T timestamp_ms;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busGenericImuFiltParams_
#define DEFINED_TYPEDEF_FOR_busGenericImuFiltParams_

// Creates a bus with parameters for generic filters for each of the 3 channels in Accels or Gyro 
struct busGenericImuFiltParams
{
  // X Axis Fiter Numerator
  real32_T xNum[3];

  // X Axis Fiter Denominator
  real32_T xDen[3];

  // Y Axis Fiter Numerator
  real32_T yNum[3];

  // Y Axis Fiter Denominator
  real32_T yDen[3];

  // Z Axis Fiter Numerator
  real32_T zNum[3];

  // Z Axis Fiter Denominator
  real32_T zDen[3];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busImuNtchFiltParams_
#define DEFINED_TYPEDEF_FOR_busImuNtchFiltParams_

// Bus Containing Notch Filter Params For IMU
struct busImuNtchFiltParams
{
  // Accel Notch Filter Params
  busGenericImuFiltParams accelNtchFilt;

  // Gyro Notch Filter Params
  busGenericImuFiltParams gyroNtchFilt;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAccelParams_
#define DEFINED_TYPEDEF_FOR_busAccelParams_

// Bus containing Accel correction Params
struct busAccelParams
{
  // accel offset correction (applied to acce value in g's)
  real32_T offset_nd[3];

  // Accel scale and alignment correction matrix
  real32_T scaleAlignMat_nd[9];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busMagParams_
#define DEFINED_TYPEDEF_FOR_busMagParams_

// Bus containing Mag correction Params
struct busMagParams
{
  // Mag offset correction
  real32_T offset_uT[3];

  // Mag scale and alignment correction matrix
  real32_T scaleAlignMat_nd[9];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busLidarParams_
#define DEFINED_TYPEDEF_FOR_busLidarParams_

// Bus containing lidar params
struct busLidarParams
{
  // X offset of Lidar mount point wrt to CG
  real32_T xMntOff_m;

  // Y offset of Lidar mount point wrt to CG
  real32_T yMntOff_m;

  // Z offset of Lidar mount point wrt to CG
  real32_T zMntOff_m;

  // Valid Lidar range, outside this range the lidar data is invalid. Index 1 min, index 2 max 
  real32_T validRange_m[2];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busMtf01pParams_
#define DEFINED_TYPEDEF_FOR_busMtf01pParams_

// MTF01P prameters for use in state estimators
struct busMtf01pParams
{
  // Rotates sensor axis to body axis
  real32_T sensorToBodyRot[4];

  // [min; max] valid range of laser range finder. Bound dist measurement between these values 
  real32_T distLimit_m[2];

  // Precision threshold below which reject the distance and flow measurement
  uint8_T distPrecisionThr;

  // Flow quality threshold below which to reject the flow measurement
  uint8_T flowQualityThr;

  // OF data filter cutoff
  real32_T filterBw_radps;

  // Position vector of the sensor from the CG
  real32_T posVector_m[3];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busStateEstSmParams_
#define DEFINED_TYPEDEF_FOR_busStateEstSmParams_

// State Estimator State Machine Parameters
struct busStateEstSmParams
{
  // Number of IMU readings to average during initialization
  real32_T imuInitCount;

  // Number of valid Mag readings to average during initialization
  real32_T magInitCount;

  // Number of valid GPS readings to average during initialization
  real32_T gpsInitCount;

  // Number of valid Baro readings to average during initialization
  real32_T baroInitCount;

  // Number of Valid GPS count before flagging GPS to be valid
  uint8_T desValidGpsCount;

  // Duration to check the GPS validity flag before flagging GPS LOSS
  real32_T gpsLossCheckDuration_s;

  // Min horizontal accuracy needed to initialze GPS into state estimator
  real32_T gpsHorAccThres_m;

  // Min vertical accuracy needed to initialze GPS into state estimator
  real32_T gpsVerAccThres_m;

  // Duration to check the OF validity flag before flagging OF LOSS
  real32_T ofLossCheckDuration_s;

  // Magnetic Declination At The Vehicle Position (usually at the take off location) 
  real32_T initMagDec_rad;

  // Flag to indicate if optical flow is in use or not
  boolean_T useOpticalFlow;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumStateEstimateMode_
#define DEFINED_TYPEDEF_FOR_enumStateEstimateMode_

// Defines the state estimate mode in use
enum class enumStateEstimateMode
  : int32_T {
  NONE = 0,                            // Default value
  INITIALIZE,
  RUN,
  RUN_GPS_NOT_INIT,
  RUN_INIT_GPS,
  RUN_GPS_LOST
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busIsAidingUsed_
#define DEFINED_TYPEDEF_FOR_busIsAidingUsed_

// Bus including flags to indicate if a sensor has been used in correction step of the EKF 
struct busIsAidingUsed
{
  // Flag to indicate if mag data was used in correction step
  boolean_T isMagUsed;

  // Integer
  uint8_T isGpsUsed;

  // Flag to indicate if Baro data was used in the correction stage
  boolean_T isBaroUsed;

  // Flag to indicate if Lidar data was used in correction step
  boolean_T isLidarUsed;
  boolean_T isFlowUsed;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumDhFifoStatus_
#define DEFINED_TYPEDEF_FOR_enumDhFifoStatus_

// Defines the Delayed Horizon FIFO status
enum class enumDhFifoStatus
  : int32_T {
  OK = 0,                              // Default value
  OVERWRITE,
  BAD_CONFIG,
  BAD_TIMESTAMP,
  TIMEBASE_RESET,
  STALE_DISCARD,
  BAD_CAPACITY
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFifoDebug_
#define DEFINED_TYPEDEF_FOR_busFifoDebug_

// Contains individual fifo debug data
struct busFifoDebug
{
  enumDhFifoStatus status;

  // Fifo data count
  uint16_T count;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busDhBufferDebug_
#define DEFINED_TYPEDEF_FOR_busDhBufferDebug_

// Delayed Horizon Buffer Manager Debug data
struct busDhBufferDebug
{
  // estimator status FIFO debug data
  busFifoDebug statusFifoDebugData;

  // IMU FIFO debug data
  busFifoDebug imuFifoDebugData;

  // mag FIFO debug data
  busFifoDebug magFifoDebugData;

  // GPS FIFO debug data
  busFifoDebug gpsFifoDebugData;

  // baro FIFO debug data
  busFifoDebug baroFifoDebugData;

  // Lidar FIFO debug data
  busFifoDebug lidarFifoDebugData;

  // flow FIFO debug data
  busFifoDebug flowFifoDebugData;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busEkfDebugData_
#define DEFINED_TYPEDEF_FOR_busEkfDebugData_

// Bus that contains EKF debug data
struct busEkfDebugData
{
  // Sensor aiding boolean flags
  busIsAidingUsed isAidingUsed;

  // Fused EKF States at delayed horizon
  real32_T dhStates[23];

  // DH buffer debug data
  busDhBufferDebug dhBufferDebugData;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busStateEstimatorDebug_
#define DEFINED_TYPEDEF_FOR_busStateEstimatorDebug_

// Bus containing debug data from state estiator
struct busStateEstimatorDebug
{
  // State Estimator initialization percentage
  real32_T stateEstInitPct;

  // State Machine Mode
  enumStateEstimateMode smMode;

  // EKF Debug Data
  busEkfDebugData ekfDebugData;
};

#endif
#endif                                 // stateEstimatorEskfAutocode_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
