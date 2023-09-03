#include <Arduino.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;

PeriodicTimer t1(GPT2);

void callback() // toggle the LED
{
    digitalWriteFast(33, !digitalReadFast(33));
}

void setup()
{
    t1.begin(callback, 250ns);

    pinMode(33, OUTPUT);
}

void loop()
{
}