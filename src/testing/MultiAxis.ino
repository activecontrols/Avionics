#include <Arduino.h>
#include "HX711.h"
#include <Wire.h>
#include <Servo.h>

// ESC variables
const int ESC_PIN = 9;
int arm_tries = 10;
const int low_endpoint = 1020;
const int high_endpoint = 1980;
const int TARE_BUTTON = 2;
const int RPM_pin = 14;

// Scales 1 - 4 are for Force. 5 is for torque
HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;
HX711 scale5;

// EDF
Servo EDF;

// Loadcell Clock and Data pins
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;

// Auto Throttling Variables
int throttle_cap;
int throttle_setting;
bool throttle_flag;
int throttle_incrememnt; 
int throttle_delay;
bool startFlag;

// Data Accquisition Variables
double thrust;
double torque;
long unsigned int timer;
const double a_thrust = -0.0141443; // Calibration for thrust load cells
const double b_thrust = 0.000449718; // Calibration for thrust load cells
const double a_torque = -0.0141443; // Calibration for torque load cells
const double b_torque = 0.000449718; // Calibration for torque load cells
double torque_arm = 0.1158875; // Perpendicular distance from point of torque to loadcell

// Function for initialising busses on multiplexor
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

// Function for initialising HX711 board
bool initHX711(HX711 scale){
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(2280.f);  // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();
  Serial.println();
  return scale.is_ready();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Initialising HX711
  // Init sensor on bus number 2
  TCA9548A(2);
  if(!initHX711(scale1)){
    Serial.print("Could not find a valid HX711 sensor on bus 2, check wiring!");
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 3
  TCA9548A(3);
  if(!initHX711(scale2)){
    Serial.print("Could not find a valid HX711 sensor on bus 3, check wiring!");
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 4
  TCA9548A(4);
  if(!initHX711(scale3)){
    Serial.print("Could not find a valid HX711 sensor on bus 4, check wiring!");
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 5
  TCA9548A(5);
  if(!initHX711(scale4)){
    Serial.print("Could not find a valid HX711 sensor on bus 5, check wiring!");
    while(1);
  }
  Serial.println();

  // Init sensor on bus number 6
  TCA9548A(6);
  if(!initHX711(scale5)){
    Serial.print("Could not find a valid HX711 sensor on bus 6, check wiring!");
    while(1);
  }
  Serial.println();

  // Initialising EDF
  EDF.attach(ESC_PIN);
  EDF.writeMicroseconds(low_endpoint);
  for (byte i = 0; i < arm_tries; i++) {
    EDF.writeMicroseconds(1000);
  }

  // Auto Throttling parameters initialisations
  throttle_setting = 0;
  //previous_throttle_setting = 0;
  throttle_cap = 100;
  throttle_flag = false; // false = increase
  throttle_incrememnt = 5; 
  throttle_delay = 2 * 1000; //s to ms 
  startFlag = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  // Wait to start code
  if(!startFlag){
    while(Serial.available() == 0){
      Serial.print("Waiting for Command Y");
      delay(2000);
    }
    if(Serial.read() == ('Y')){
      EDF.write(low_endpoint);
      Serial.print("Starting");
      delay(10000);
      startFlag = true;
      } else {
        Serial.println("Incorrect start variable");
        while(1); // need to start if stuck here
      }
  }


  // Auto Throttling
  if(!throttle_flag){
    throttle_setting = throttle_setting + throttle_incrememnt;
    if(throttle_setting == throttle_cap){
      throttle_flag = true;
    }
    writeThrottle(throttle_setting);
    timer = millis();
    while(millis() < timer + throttle_delay){
      thrust = a_thrust*(scale1.get_units() + scale2.get_units() + scale3.get_units() + scale4.get_units()) + b_thrust;
      torque = (a_torque*scale5.get_units() + b_torque) * torque_arm;
      printData(millis(), throttle_setting, thrust, torque, analogRead(RPM_pin));
    }
  } else {
    throttle_setting = throttle_setting - throttle_incrememnt;
    if(throttle_setting < 0){
      while(1){
        Serial.print("EXECTUION ENDED\n");
        EDF.writeMicroseconds(low_endpoint);
        delay(2000);
      }
    }
    writeThrottle(throttle_setting);
    timer = millis();
    while(millis() < (timer + throttle_delay)){
      thrust = a_thrust*(scale1.get_units() + scale2.get_units() + scale3.get_units() + scale4.get_units()) + b_thrust;
      torque = (a_torque*scale5.get_units() + b_torque) * torque_arm;
      printData(millis(), throttle_setting, thrust, torque, analogRead(RPM_pin));
    }
    // Code to stop arduino from exectuing anything else
  }

}

// Functions

bool writeThrottle(int throttle_setting){
  int throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint);
  if((throttle >= low_endpoint) && (throttle <= high_endpoint)){
    EDF.writeMicroseconds(throttle);
    return true;
  } else {
    if(throttle_setting < 0){
      throttle_setting = 0;
      throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint); 
      EDF.writeMicroseconds(throttle);
    } else {
      //EMERGENCY THROTTLE DOWN
      while(throttle_setting > 0){
        delay(2000);
        throttle_setting = throttle_setting - 30;
        if(throttle_setting < 0 ){
          throttle_setting = 0;
        }
        throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint); 
        EDF.writeMicroseconds(throttle);
      }
    }
    while(1){
      Serial.print("INVALID THROTTLE\n");
    }
    return false;
  }
}

bool endCode(int throttle_setting){
  if(throttle_setting < 0){
    while(1){
      Serial.print("Execution Ended\n");
    }
    return false;
  }
  return true;
}

void printData(int time, int throttle, double thrust, double torque, int RPM){
  Serial.print(time);
  Serial.print(",");
  Serial.print(throttle);
  Serial.print(",");
  String s = String(thrust,3);
  Serial.print(s);
  Serial.print(",");
  s = String(torque,3);
  Serial.print(s);
  Serial.print(",");
  Serial.print(RPM);
  Serial.print("\n");
}
