
#ifndef IMU_H
#define IMU_H

#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_AHRS.h>

#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>

//uncomment to print data to console for just IMU
//#define ASTRA_IMU_DEBUG 
#define FILTER_UPDATE_RATE_HZ 100
#define PRINT_EVERY_N_UPDATES 10


// Full orientation sensing using NXP/Madgwick/Mahony and a range of 9-DoF
// sensor sets.
// You *must* perform a magnetic calibration before this code will work.
//
// To view this data, use the Arduino Serial Monitor to watch the
// scrolling angles, or run the OrientationVisualiser example in Processing.
// Based on  https://github.com/PaulStoffregen/NXPMotionSense with adjustments
// to Adafruit Unified Sensor interface



#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
  static Adafruit_Sensor_Calibration_EEPROM cal;
#else
  Adafruit_Sensor_Calibration_SDFat cal;
#endif

#define FILTER_UPDATE_RATE_HZ 100
#define PRINT_EVERY_N_UPDATES 10
//#define AHRS_DEBUG_OUTPUT


extern float roll, pitch, heading;
extern float gx, gy, gz; //degrees per second on gyro
extern float qw, qx, qy, qz; //quaternarion
static uint32_t timestamp;

// slower == better quality output
static Adafruit_NXPSensorFusion filter; // slowest

extern int setupIMU();
extern int updateIMU();

#endif



