#include <Arduino.h>

double measurement1;
double measurement2;
double measurement3;
double measurement4;
double measurement5;

double weightsA1 = 0.0084914244;
double weightsA2 = -0.0182792591;
double weightsA3 = 0.0115852731;
double weightsA4 = 0.0051548949;

int torqueSensor = A4;
int thrustSensor1 = A0;
int thrustSensor2 = A3;
int thrustSensor3 = A1;
int thrustSensor4 = A2;
double mass = 3;

double temp1 = 0;
double temp2 = 0;
double temp3 = 0;
double temp4 = 0;
double temp5 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(torqueSensor, INPUT);
  pinMode(thrustSensor1, INPUT);
  pinMode(thrustSensor2, INPUT);
  pinMode(thrustSensor3, INPUT);
  pinMode(thrustSensor4, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  temp1 = 0;
  temp2 = 0;
  temp3 = 0;
  temp4 = 0;
  temp5 = 0;

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
  Serial.println(mass);
  delay(20);
}
