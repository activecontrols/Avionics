#include "IMU.h"

void setup() {
    setupIMU();
}

void loop() {
    updateIMU();
    Serial.print("test");
    Serial.print(heading);
    
}