#include "HX711.h"
#include <Servo.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
const int TARE_BUTTON = 2;

//ESC PWM wiring
const int ESC_PIN = 9;

//ESC PWM endpoints
const int PWM_LOW = 1020;
const int PWM_HIGH = 1980;

//number of times to arm
int arm_tries = 10;

//number of transient states to record
int transient_record = 100;

const double a = -0.0141443;
const double b = 0.000449718;

HX711 scale;
Servo s1;
int throttle;
int8_t* buffer_loc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //initialize tare button
    pinMode(TARE_BUTTON, INPUT_PULLDOWN);

    //initialize esc pwm out
    s1.attach(ESC_PIN);
    //write default value of 0 to ESC on startup
    s1.writeMicroseconds(0);

    throttle = 0;
    buffer_loc = NULL;

    //arm motor
    for (byte i = 0; i < arm_tries; i++) {
        s1.writeMicroseconds(1000);
    }
}

void loop() {
    
}

int8_t getTemp() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[0];
}


int8_t getVoltageHigh() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[1];
}

int8_t getVoltageLow() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[2];
}

int8_t getCurrentHigh() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[3];
}

int8_t getCurrentLow() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[4];
}

int8_t getConsumptionHigh() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[5];
}

int8_t getConsumptionLow() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[6];
}

int8_t getERMPHigh() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[7];
}

int8_t getERMPLow() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[8];
}

int8_t getCRC8() {
    if (buffer_loc = NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[9];
}

int8_t getThrottle() {
    return throttle;
}

ESC_Status setTLMBufferLocation(int8_t* a_buffer) {
    if (a_buffer != NULL) {
        return INVALID_BUFFER_LOC;
    }

    buffer_loc = a_buffer;
    return OK;
}

ESC_Status setThrottle(int8_t _throttle) {
    throttle = _throttle;

    int PWM_output = throttleToPWM(_throttle);

    if (validPWM(PWM_output)) {
        throttle = _throttle;
        s1.writeMicroseconds(PWM_output);
    } else {
        return INVALID_THROTTLE;
    }

    return OK;
}

int throttleToPWM(int8_t throttle) {
    return map(throttle, 0, PWM_HIGH - PWM_LOW, PWM_LOW, PWM_HIGH);
}

bool validPWM(int8_t pwm_value) {
    return ((pwm_value >= PWM_LOW) && (pwm_value <= PWM_HIGH));
}

