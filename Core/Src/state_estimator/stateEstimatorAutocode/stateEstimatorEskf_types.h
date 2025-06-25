//
// File: stateEstimatorEskf_types.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.64
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Tue Jun 24 17:45:12 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_stateEstimatorEskf_types_h_
#define RTW_HEADER_stateEstimatorEskf_types_h_
#include "rtwtypes.h"

// Model Code Variants
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

#ifndef DEFINED_TYPEDEF_FOR_busImuData_
#define DEFINED_TYPEDEF_FOR_busImuData_

// Bus containing accelerometer and gyro data in body axis
struct busImuData
{
  // Acceleration in body axis including gravity
  real32_T bodyAccels_mps2[3];

  // Body angular rates from gyro
  real32_T bodyRates_radps[3];
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
  // Y offset of Lidar mount point wrt to CG
  real32_T yMntOff_m;

  // Z offset of Lidar mount point wrt to CG
  real32_T zMntOff_m;

  // Valid Lidar range, outside this range the lidar data is invalid. Index 1 min, index 2 max 
  real32_T validRange_m[2];
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

  // Magnetic Declination At The Vehicle Position (usually at the take off location) 
  real32_T initMagDec_rad;
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
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_vlrWXdgCyensSoz7WQOtZH_
#define DEFINED_TYPEDEF_FOR_struct_vlrWXdgCyensSoz7WQOtZH_

struct struct_vlrWXdgCyensSoz7WQOtZH
{
  real32_T nisNedPosAndVel[6];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_lNBPdDZBS05BfQyyvsjhxE_
#define DEFINED_TYPEDEF_FOR_struct_lNBPdDZBS05BfQyyvsjhxE_

struct struct_lNBPdDZBS05BfQyyvsjhxE
{
  uint8_T numStates;
  uint8_T numErrorStates;
  uint8_T numMeas;
  struct_vlrWXdgCyensSoz7WQOtZH nisParams;
};

#endif
#endif                                // RTW_HEADER_stateEstimatorEskf_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
