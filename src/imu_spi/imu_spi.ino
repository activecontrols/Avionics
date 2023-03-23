#include <SPI.h>
#include <Adafruit_LSM6DSOX.h>

// Initialize the SPI communication
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);
Adafruit_LSM6DSOX lsm6dsox;

void setup() {
  // Start the SPI communication
  SPI.begin();
  SPI.beginTransaction(spiSettings);
  
  // Initialize the LSM6DSOX
  if (!lsm6dsox.begin_SPI()) {
    Serial.println("Failed to initialize LSM6DSOX!");
    while (1);
  }
  
  // Configure the LSM6DSOX
  lsm6dsox.setAccelDataRate(LSM6DSOX_ACCEL_DATARATE_104HZ);
  lsm6dsox.setGyroDataRate(LSM6DSOX_GYRO_DATARATE_104HZ);
  lsm6dsox.setAccelRange(LSM6DSOX_ACCEL_RANGE_2_MG);
  lsm6dsox.setGyroRange(LSM6DSOX_GYRO_RANGE_250_DPS);
  lsm6dsox.setFilterBandwidth(LSM6DSOX_BANDWIDTH_AUTO);

  // Print out LSM6DSOX chip ID
  // Serial.print("LSM6DSOX chip ID: ");
  // Serial.println(lsm6dsox.chipID());
}

void loop() {
  // Read accelerometer data
  sensors_event_t accelEvent;
  lsm6dsox.getEvent(&accelEvent, Adafruit_LSM6DSOX::ACCEL_SENSOR);
  Serial.print("Accel: ");
  Serial.print(accelEvent.acceleration.x);
  Serial.print(", ");
  Serial.print(accelEvent.acceleration.y);
  Serial.print(", ");
  Serial.println(accelEvent.acceleration.z);

  // Read gyroscope data
  sensors_event_t gyroEvent;
  lsm6dsox.getEvent(&gyroEvent, Adafruit_LSM6DSOX::GYRO_SENSOR);
  Serial.print("Gyro: ");
  Serial.print(gyroEvent.gyro.x);
  Serial.print(", ");
  Serial.print(gyroEvent.gyro.y);
  Serial.print(", ");
  Serial.println(gyroEvent.gyro.z);

  delay(100);
}