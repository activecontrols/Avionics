#include "IMU.h"
#include "../../error/DriverError.h"
#include "../include/LSM6DS_LIS3MDL.h"



float roll, pitch, heading = 0;
float gx, gy, gz = 0; //degrees per second on gyro
float qw, qx, qy, qz = 0; //quaternarion

int setupIMU() {
  Serial.begin(115200);
  while (!Serial) yield();

  if (!cal.begin()) {
    //Failed to initialize calibration helper
    Serial.print("Failed to initialize calibration helper");
    return FAILED_CALIBRATION_HELPER_INIT;
  } else if (!cal.loadCalibration()) {
    //No calibration loaded/found
    Serial.println("No calibration loaded/found");
    return NO_CALIBRATION_FOUND;
  }

  if (!init_sensors()) {
    //Failed to find sensors
    return FAILED_SENSOR_INIT;
  }
  
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
  accelerometer->printSensorDetails();
  gyroscope->printSensorDetails();
  magnetometer->printSensorDetails();
#endif

  setup_sensors();
  filter.begin(FILTER_UPDATE_RATE_HZ);
  timestamp = millis();

  Wire.setClock(400000); // 400KHz

  return NO_ERROR_CODE;
}

int updateIMU() {
  // float roll, pitch, heading;
  // float gx, gy, gz;

  static uint8_t counter = 0;

  // if ((millis() - timestamp) < (1000 / FILTER_UPDATE_RATE_HZ)) {
  //   return;
  // }
  timestamp = millis();
  // Read the motion sensors
  sensors_event_t accel, gyro, mag;
  accelerometer->getEvent(&accel);
  gyroscope->getEvent(&gyro);
  magnetometer->getEvent(&mag);

#if defined(AHRS_DEBUG_OUTPUT)
  Serial.print("I2C took "); Serial.print(millis()-timestamp); Serial.println(" ms");
#endif

  cal.calibrate(mag);
  cal.calibrate(accel);
  cal.calibrate(gyro);

  // Gyroscope needs to be converted from Rad/s to Degree/s
  // the rest are not unit-important
  gx = gyro.gyro.x * SENSORS_RADS_TO_DPS;
  gy = gyro.gyro.y * SENSORS_RADS_TO_DPS;
  gz = gyro.gyro.z * SENSORS_RADS_TO_DPS;

  // Update the SensorFusion filter
  filter.update(gx, gy, gz, 
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, 
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);
#if defined(AHRS_DEBUG_OUTPUT)
  Serial.print("Update took "); Serial.print(millis()-timestamp); Serial.println(" ms");
#endif

  // only print the calculated output once in a while
  // if (counter++ <= PRINT_EVERY_N_UPDATES) {
  //   return;
  // }
  // reset the counter
  //counter = 0;

#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
  Serial.print("Raw: ");
  Serial.print(accel.acceleration.x, 4); Serial.print(", ");
  Serial.print(accel.acceleration.y, 4); Serial.print(", ");
  Serial.print(accel.acceleration.z, 4); Serial.print(", ");
  Serial.print(gx, 4); Serial.print(", ");
  Serial.print(gy, 4); Serial.print(", ");
  Serial.print(gz, 4); Serial.print(", ");
  Serial.print(mag.magnetic.x, 4); Serial.print(", ");
  Serial.print(mag.magnetic.y, 4); Serial.print(", ");
  Serial.print(mag.magnetic.z, 4); Serial.println("");
#endif

  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();
  Serial.print("Orientation: ");
  Serial.print(heading);
  Serial.print(", ");
  Serial.print(pitch);
  Serial.print(", ");
  Serial.println(roll);

  //float qw, qx, qy, qz;
  filter.getQuaternion(&qw, &qx, &qy, &qz);
  Serial.print("Quaternion: ");
  Serial.print(qw, 4);
  Serial.print(", ");
  Serial.print(qx, 4);
  Serial.print(", ");
  Serial.print(qy, 4);
  Serial.print(", ");
  Serial.println(qz, 4);  
  
#if defined(AHRS_DEBUG_OUTPUT)
  Serial.print("Took "); Serial.print(millis()-timestamp); Serial.println(" ms");
#endif

  return NO_ERROR_CODE;

}

