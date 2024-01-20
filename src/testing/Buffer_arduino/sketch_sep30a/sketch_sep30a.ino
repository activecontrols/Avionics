#include <MS5611.h>
#include "buffer.h"

MS5611 baro;
int32_t pressure;
buffer test(10);

void setup() {
  // Start barometer
  baro = MS5611();
  baro.begin();
  // Start serial 
  Serial.begin(9600);
  delay(2);
}

void loop() {
  // Read pressure
  pressure = baro.getPressure();
  // Send pressure 
  float* x;
  Serial.println(pressure);
  if(test.record()){
    x = test.getValue();
    for(int i = 0; i++; i < 10){
      Serial.print(*x);
      x = x++;
    }
  };
}