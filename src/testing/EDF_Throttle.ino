#include "HX711.h"
#include <Servo.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
const int TARE_BUTTON = 2;

//ESC PWM wiring
const int ESC_PIN = 9;

//ESC PWM endpoints
const int low_endpoint = 1020;
const int high_endpoint = 1980;

//number of times to arm
int arm_tries = 10;

//number of transient states to record
int transient_record = 100;

const double a = -0.0141443;
const double b = 0.000449718;

HX711 scale;
Servo s1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //initialize tare button
  pinMode(TARE_BUTTON, INPUT);

  //initialize esc pwm out
  s1.attach(ESC_PIN);
  //write default value of 0 to ESC on startup
  s1.writeMicroseconds(0);

  //initialize scale on scale output pins
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //set scale output settings
  scale.set_scale(2280.f);  // this value is obtained by calibrating the scale with known weights; see the README for details
  //tare scale on startup
  scale.tare();
  Serial.println("Time (ms),Throttle %,Thrust Force (N)");

  //arm motor
  for (byte i = 0; i < arm_tries; i++) {
    s1.writeMicroseconds(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //if tare button is pressed, tare scale
//  if (digitalRead(TARE_BUTTON)) {
//    Serial.println("Tared scale");
//    scale.tare();
//  }

  //read command from user input on serial
  if (Serial.available() > 0) {
    // read the incoming byte:
    int throttle_setting = Serial.readString().toInt();
    if(throttle_setting >= 20){
      throttle_setting = 20;
    }
    int PWM_output = throttleToPWM(throttle_setting);

    if (validPWM(PWM_output)) {
      s1.writeMicroseconds(PWM_output);

      for (int i = 0; i < transient_record; i++) {
        int time = millis();
        double raw_reading = scale.get_units();
        double force = (a * raw_reading + b);
        Serial.print(time);
        Serial.print(",");
        Serial.print(throttle_setting);
        Serial.print(",");
        Serial.println(force, 3);
      }
    }
  }
}

int throttleToPWM(int throttle) {
  return map(throttle, 0, 100, low_endpoint, high_endpoint);
}

bool validPWM(int PWM_out) {
  return ((PWM_out >= low_endpoint) && (PWM_out <= high_endpoint));
}
