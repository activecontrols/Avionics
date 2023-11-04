#include "IMU.h"

void setup() {
    initalizeIMU();
}

void loop() {
    updateIMU();
    Serial.print("test");
    Serial.print(heading);
}