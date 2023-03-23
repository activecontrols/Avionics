#include <Wire.h>
#include <Adafruit_Sensor.h>

// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor

#include <Adafruit_LSM6DSOX.h>

Adafruit_LSM6DSOX imu = Adafruit_LSM6DSOX();

// For SPI mode, we need a CS pin
#define LSM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11

void setup(void) {
  Wire.begin();
  imu.begin_I2C();
  
}

void loop() {
  sensors_event_t a, g, temp;
  imu.getEvent(&a, &g, &temp);

  // Use the readings as needed.
  float x_accel = a.acceleration.x;
  float y_accel = a.acceleration.y;
  float z_accel = a.acceleration.z;
  // Serial.print("Accelerometer data: X=");
  Serial.print(x_accel);
  Serial.print(",");
  Serial.print(y_accel);
  Serial.print(",");
  Serial.print(z_accel);

  float x_gyro = g.gyro.x;
  float y_gyro = g.gyro.y;
  float z_gyro = g.gyro.z;
  Serial.print(",");
  Serial.print(x_gyro);
  Serial.print(",");
  Serial.print(y_gyro);
  Serial.print(",");
  Serial.println(z_gyro);

  delay(100);

  // Do something with the readings...
}