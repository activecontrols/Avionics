#ifndef buffer_h

#define buffer_h
#include "Arduino.h"

class buffer
{
public:
    buffer(int size);
    bool record();
    float* getValue();
    //bool refresh();
private:
    float* data;
};

#endif