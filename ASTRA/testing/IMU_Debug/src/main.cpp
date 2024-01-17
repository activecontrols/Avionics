#include "../../../IMU/src/IMU.h"

/*
main.cpp
Description: Main file for running testing code for the IMU driver
Author: Vincent Palmerio
*/

Adafruit_LSM6DSOX sox;

void setup() {
    initializeIMU();

    if (!sox.begin_I2C()) {
        // if (!sox.begin_SPI(LSM_CS)) {
        // if (!sox.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
        // Serial.println("Failed to find LSM6DSOX chip");
        while (1) {
            delay(10);
        }
    }
}

void loop() {

    updateIMU();
    Serial.print("Fusion values: ");
    Serial.print(yaw);
    Serial.print(" ");
    Serial.print(roll);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.println();


    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    sox.getEvent(&accel, &gyro, &temp);

    Serial.print(millis(),7);
    Serial.print(",");

    Serial.print(temp.temperature,7);
    Serial.print(",");

    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print(accel.acceleration.x,7);
    Serial.print(",");
    Serial.print(accel.acceleration.y,7);
    Serial.print(",");
    Serial.print(accel.acceleration.z,7);
    Serial.print(",");

    /* Display the results (rotation is measured in rad/s) */
    Serial.print(gyro.gyro.x,7);
    Serial.print(",");
    Serial.print(gyro.gyro.y,7);
    Serial.print(",");
    Serial.print(gyro.gyro.z,7);
    Serial.println();
    
}