#include <Arduino.h>
#include "ESCCMD.h"

//MAKE SURE TO UNCOMMENT LINE 16 of ESCCMD.cpp BEFORE RUNNING!

uint16_t volt = 10;
uint8_t deg;
uint16_t amp;
uint16_t mah;
int16_t rpm;

void setup() {
  // put your setup code here, to run once:
  ESCCMD_init(0);
  ESCCMD_arm_all();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ESCCMD_throttle(0, 1000);

  ESCCMD_read_deg(0, &deg);
  ESCCMD_read_volt(0, &volt);
  ESCCMD_read_amp(0, &amp);
  ESCCMD_read_rpm(0, &rpm);
  ESCCMD_read_mah(0, &mah);

  Serial.print("Temp: "); Serial.println(deg);
  Serial.print("Volt: "); Serial.println(volt);
  Serial.print("Amp: "); Serial.println(amp);
  Serial.print("RPM: "); Serial.println(rpm);
  Serial.print("mAh: "); Serial.println(mah);

  delay(500);
}