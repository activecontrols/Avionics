#ifndef ESC_H

#define ESC_H


enum ESC_Status {
    OK,
    INVALID_THROTTLE,
    INVALID_BUFFER_LOC
};

// getters for TLM data
int getTemp();
int getVoltageHigh();
int getVoltageLow();
int getCurrentHigh();
int getCurrentLow();
int getConsumptionHigh();
int getConsumptionLow();
int getERMPHigh();
int getERMPLow();
int getCRC8();

int getThrottle();

ESC_Status setThrottle();
ESC_Status setTLMBufferLocation();

int throttleToPWM(int throttle);
bool validPWM(int PWM_out);

#endif