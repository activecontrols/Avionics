#ifndef buffer_h

#define buffer_h
#include "Arduino.h"

class Queue
{
public:
    Queue(int m, int n);
    add(float* x);
private:
    float* data;
};

#endif