#include "HX711.h"
#include <Servo.h>
#include "esc.h"


String input;
int throttle_setting;
int previous_throttle_setting;
int ESC_PIN;


HX711 scale;
Servo EDF;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    ESC_PIN = 9;
    // initialize tare button
    pinMode(TARE_BUTTON, INPUT_PULLDOWN);

    ESC esc = ESC(ESC_PIN);
    throttle_setting = 0;
    previous_throttle_setting = 0;


}

void loop(){
    if(Serial.available() > 0){
        input = Serial.readString();
    } else {
        Serial.print(" ");
    }
}