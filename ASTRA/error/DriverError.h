/*
DriverError.h - Error Header/Constants File for Drivers
Description: Error Constants for all Driver Errors that occur
Author: Vincent Palmerio
*/

#ifndef DRIVER_ERROR_H
#define DRIVER_ERROR_H

#define NO_ERROR_CODE 0
#define GENERAL_ERROR_CODE -1
#define MEMORY_ALLOCATION_ERROR_CODE -2

//IMU Error codes are in the 100's
#define FAILED_CALIBRATION_HELPER_INIT -100
#define FAILED_LOAD_CALIBRATION -101
#define FAILED_SENSOR_INIT -102


#endif