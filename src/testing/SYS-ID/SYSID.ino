#include <Servo.h>
#include <Math.h>


// SENSOR CODE
double measurement1;
double measurement2;
double measurement3;
double measurement4;
double measurement5;

int torqueSensor = A4;
int thrustSensor1 = A0;
int thrustSensor2 = A3;
int thrustSensor3 = A1;
int thrustSensor4 = A2;
double temp1 = 0;
double temp2 = 0;
double temp3 = 0;
double temp4 = 0;
double temp5 = 0;

double weightsA1 = 0.0084914244;
double weightsA2 = -0.0182792591;
double weightsA3 = 0.0115852731;
double weightsA4 = 0.0051548949;
double mass;


// SYS-ID CODE

Servo vane1;
Servo vane2;
Servo EDF;
int vane1Pin = 2;
int vane2Pin = 2;
int EDFPin = 2
int thrustPin1 = 1;
int thrustPin2 = 1;
int thrustPin3 = 1;
int thrustPin4 = 1;
int torquePin1 = 1;

int totalVaneRange = 16; // Total vane range in degrees
int maxVaneRange = 8; // Max vane angle in degrees
int baseVaneSetting = 90; // Initial Vane setting in degrees

const int low_endpoint = 1020; // 0 throttle = 1000
const int high_endpoint = 1980; // 100 throttle = 2000
int delta = high_endpoint - low_endpoint;
int arm_tries = 10;
bool startFlag = false;

/*

NOTE FOR VANE CONTROL. 0 Corresponds to - 8 ddegrees, and 1 corresponds to + 8 degrees

*/


int baseTime;
int duration = 10 * 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vane1.attach(vane1Pin);
  vane2.attach(vane2Pin);
  EDF.attach(EDFPin);
  vane1.write(90);
  vane2.write(90);
  EDF.writeMicroseconds(1020);
   for (byte i = 0; i < arm_tries; i++) {
    EDF.writeMicroseconds(1000);
  }

  pinMode(torqueSensor, INPUT);
  pinMode(thrustSensor1, INPUT);
  pinMode(thrustSensor2, INPUT);
  pinMode(thrustSensor3, INPUT);
  pinMode(thrustSensor4, INPUT);
}
}

void loop() {
  // put your main code here, to run repeatedly:
  temp1 = 0;
  temp2 = 0;
  temp3 = 0;
  temp4 = 0;
  temp5 = 0;

  delay(10);
  if(!startFlag){
    while(Serial.available() == 0){
    Serial.print("Waiting for Command Y");
    delay(2000);
    }
  if(Serial.read() == ('Y')){
    EDF.write(1020);
    Serial.print("Starting");
    delay(5000);
    startFlag = true;
    }
  }
  delay(10*1000);
  baseTime = millis();
  while(millis() - baseTime < duration){
    
    delay(5);
    vane1.write(initialVaneSetting + ramp(baseTime, duration)*totalVaneRange - maxVaneRange);
    Serial.println("Ramp");
    Serial.println(ramp(baseTime, duration));
    vane2.write(initialVaneSetting + ramp(baseTime, duration)*totalVaneRange - maxVaneRange);
    Serial.println(parabola(baseTime, duration));
    EDF.writeMicroseconds(low_endpoint + ramp(baseTime,duration)*delta);
  }
  mass = getMass();
  Serial.println(mass);  
  delay(5);



}

void unitStep(int throttle, Servo object, char identifier){
  int preValue = 0;
  int counter = 0;
  int thrust = 0;
  do{
    preValue = thrust;
  if(identifier == 'e'){
    object.writeMicroseconds(low_endpoint + (float)throttle/100*delta);
  }else{
    object.write(initalVaneSetting + throttle/100 *totalVaneRange - maxVaneRange);
  }
  thrust = getMass();
  delay(5);

  if(abs(preValue - thrust)/thrust > 0.03*thrust){
    counter++;
  } else{
    counter = 0;
  }

  }while(counter < 5);
  
}

double getMass(){
  double mass;
  for(int i = 0; i < 10; i++){
    measurement1 = analogRead(thrustSensor1) * weightsA1;
    temp1 = temp1 + measurement1;

    measurement2 = analogRead(thrustSensor2) * weightsA2;
    temp2 = temp2 + measurement2;
    
    measurement3 = analogRead(thrustSensor3) * weightsA3;
    temp3 = temp3 + measurement3;
    
    measurement4 = analogRead(thrustSensor4)*weightsA4;
    temp4 = temp4 + measurement4;

    measurement5 = analogRead(torqueSensor);
    temp5 = temp5 + measurement5;
    delay(20);
  }
  temp1 = temp1/10;
  temp2 = temp2/10;
  temp3 = temp3/10;
  temp4 = temp4/10;
  temp5 = temp5/10;
  mass = temp1 + temp2 + temp3 + temp4;
  return (mass);
}


float ramp(int time_base, int duration){
  float input;
  int time = millis(); 
  time = time - time_base;
  return ((float)time/duration);
    
  // Map time to input value

  // Enter your servo write function here
}


float parabola(int time_base, int duration){
  int time = millis(); 
  time = time - time_base;
  return ((float)time*time/duration/duration);
}


void sine(int time_base, int duration, int val, Servo object){
  float input;
  int time = millis(); 
  time = time - time_base;
  while(time <= duration){
    time = millis(); 
    time = time - time_base;
    input = time - pow(time,3)/factorial(3) + pow(time,5)/factorial(5) - pow(time,7)/factorial(7);
    object.writeMicroseconds(low_endpoint + input/duration*delta);
    // Map time to input value

    // Enter your servo write function here

  }
}

void cos(int time_base, int duration, int val, Servo object){
  float input;
  int time = millis(); 
  time = time - time_base;
  while(time <= duration){
    time = millis(); 
    time = time - time_base;
    input = 1 - pow(time,2)/factorial(2) + pow(time,4)/factorial(4) - pow(time,6)/factorial(6);
    object.writeMicroseconds(low_endpoint + input/duration*delta);

    // Map time to input value

    // Enter your servo write function here

  }
}


double pow(float x, int y){
  double num = 1;
  for(int i = 0; i < y; i++){
    num = num*x;
  }
  return num;
}

int factorial(int x){
  int num = x;
  while(x != 1){
    x = x - 1;
    num = num*x;
  }
  return num;
}
