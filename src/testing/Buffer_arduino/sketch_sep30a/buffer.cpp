#include "buffer.h"
#include "Arduino.h"
#include "HardwareSerial.h"


buffer::buffer(int size){
  float Data[10];
    data = &Data[0];
}

bool buffer::record(){
  if(Serial.available() > 0){
  float input = Serial.read();
  int i = 0;
  while(*data != 0){
    i++;
    data = data+1;
    if(i > 9){
      return false;
    }
  }
  *data = input;
  return true;
  }
  return false;
}

float* buffer::getValue(){
  return data;
}
/*int main(void){
    buffer b(10);
    return 0;
}*/
