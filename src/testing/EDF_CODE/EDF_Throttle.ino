#include "HX711.h"
#include <Servo.h>
#include <SD.h>
#include <SPI.h>
#include <string.h>
#include <iostream>
using namespace std;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
const int TARE_BUTTON = 2;
const int RPM_pin = 14;

// Variables used
double torque_arm = 0.1158875;
File testData; // File to record test data in
long unsigned int timer; // timer to mark throttle delay
double raw_reading; // raw force reading
double force; // adjusted force reading
int throttle_setting; // throttle setting
int RPM_reading; // RPM telemetry
int PWM_output; // PWN output to throttle 
int throttle_cap; // Max Throttle we want to reach
int throttle_incrememnt; // Step increment to reach max throttle and then come back to 0
long unsigned int throttle_delay; // Time between each throttle increment/decrement in ms
bool throttle_flag; // False = throttling up (increase), true = opposite
bool startFlag;

// Variables used for manual throttling (commented out)
int previous_throttle_setting; 
String user_input;
//int time;



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

  // Set the file up
  //testData = SD.open("testData.txt", FILE_WRITE); 
  //testData.write("----- Test Data -----\n");
  //initialize tare button
  //pinMode(TARE_BUTTON, INPUT);

  //initialize esc pwm out
  s1.attach(ESC_PIN);
  //write default value of 0 to ESC on startup
  s1.writeMicroseconds(1020);

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
  throttle_setting = 0;
  //previous_throttle_setting = 0;
  throttle_cap = 100;
  throttle_flag = false; // false = increase
  throttle_incrememnt = 5; 
  throttle_delay = 2 * 1000; //s to ms 
  startFlag = false;
}

void loop() {
  
  if(!startFlag){
    while(Serial.available() == 0){
    Serial.print("Waiting for Command Y");
    delay(2000);
    }
  if(Serial.read() == ('Y')){
    s1.write(1020);
    Serial.print("Starting");
    delay(10000);
    startFlag = true;
    }
  }
  
  // put your main code here, to run repeatedly:

  //if tare button is pressed, tare scale
//  if (digitalRead(TARE_BUTTON)) {
//    Serial.println("Tared scale");
//    scale.tare();
//  }
  //read command from user input on serial
  /*if (Serial.available() > 0) {
    previous_throttle_setting = throttle_setting;
    // read the incoming byte:
    user_input = Serial.readString();
    Serial.println("THROTTLE INPUT: ");
    Serial.print(",");
    Serial.print(throttle_setting);
    Serial.println();
    throttle_setting = user_input.toInt();
    //Capping throttle at a given value
    if(throttle_setting >= throttle_cap){
      throttle_setting = throttle_cap;
    }
    int throttle_diff = throttle_setting - previous_throttle_setting;

    if(abs(throttle_diff > 20)){
      int signOfX = (throttle_diff > 0) - (throttle_diff < 0);
      throttle_setting = throttle_setting + throttle_diff * (throttle_setting - previous_throttle_setting) * 20;
    }
    */

    if(!throttle_flag){
      throttle_setting = throttle_setting + throttle_incrememnt;
      if(throttle_setting >= throttle_cap){
        throttle_flag = true;
      }
      writeThrottle(throttle_setting);
      timer = millis();
      while(millis() < timer + throttle_delay){
        //time = millis();
        //raw_reading = scale.get_units();
        //force = (a * raw_reading + b);
        //RPM_reading = analogRead(RPM_pin);
        printData(millis(), throttle_setting, (a*scale.get_units() + b) * torque_arm, analogRead(RPM_pin));
      }
    } else {
      throttle_setting = throttle_setting - throttle_incrememnt;
      if(throttle_setting < 0){
        while(1){
          Serial.print("EXECTUION ENDED\n");
          s1.writeMicroseconds(low_endpoint);
          delay(2000);
        }
      }
      writeThrottle(throttle_setting);
      timer = millis();
      while(millis() < (timer + throttle_delay)){
        printData(millis(), throttle_setting, (a*scale.get_units() + b)*torque_arm, analogRead(RPM_pin));
      }
      // Code to stop arduino from exectuing anything else
    }
}


// Functions

int throttleToPWM(int throttle) {
  return map(throttle, 0, 100, low_endpoint, high_endpoint);
}

bool validPWM(int PWM_out) {
  return ((PWM_out >= low_endpoint) && (PWM_out <= high_endpoint));
}

bool writeThrottle(int throttle_setting){
  int throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint);
  if((throttle >= low_endpoint) && (throttle <= high_endpoint)){
    s1.writeMicroseconds(throttle);
    return true;
  } else {
    if(throttle_setting < 0){
      throttle_setting = 0;
      throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint); 
      s1.writeMicroseconds(throttle);
    } else {
      //EMERGENCY THROTTLE DOWN
      while(throttle_setting > 0){
        delay(2000);
        throttle_setting = throttle_setting - 30;
        if(throttle_setting < 0 ){
          throttle_setting = 0;
        }
        throttle = map(throttle_setting, 0, 100, low_endpoint, high_endpoint); 
        s1.writeMicroseconds(throttle);
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

void printData(int time, int throttle, double force, int RPM){
  Serial.print(time);
  Serial.print(",");
  Serial.print(throttle);
  Serial.print(",");
  String s = String(force,3);
  Serial.print(s);
  Serial.print(",");
  Serial.print(RPM);
  Serial.print("\n");
}
