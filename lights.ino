#include <Arduino.h>
#include "TeensyThreads.h"

// Define some LED Pins
#define LED_RED 39
#define LED_RED_FLASHMS 150

#define LED_GREEN 40
#define LED_GREEN_FLASHMS 160

//volatile int incr = 1;
//volatile int ct = 0;


// #define LED_BLUE 41
// #define LED_BLUE_FLASHMS  170

void f1(){
	while (true) {
    unsigned long startTime = micros();

	//	digitalWrite(LED_RED, HIGH);
	//	threads.delay(LED_RED_FLASHMS);
	//	digitalWrite(LED_RED, LOW);
	//	threads.delay(LED_RED_FLASHMS);

     // ct += incr;
   // threads.yield();
    unsigned long endTime = micros();
    unsigned long elapsedTime = endTime - startTime; // calculate the elapsed time in microseconds
    //Serial.print(ct);

    delay(0.1-(elapsedTime/1000000));
    threads.yield();

    //Serial.print("Elapsed time2: ");
    //Serial.print(elapsedTime);
    //Serial.print(" microseconds");
    //Serial.print(1-(elapsedTime/1000000));
    //Serial.print("\n");
	}
}

// void blinkBlue() {
// 	while (true) {
// 		digitalWrite(LED_BLUE, HIGH);
// 		threads.delay(LED_BLUE_FLASHMS);
// 		digitalWrite(LED_BLUE, LOW);
// 		threads.delay(LED_BLUE_FLASHMS);
// 		threads.yield();
// 	}
// }

void f2() {
  
	while (true) {
    unsigned long startTime = micros();
		// digitalWrite(LED_GREEN, HIGH);
		// //threads.delay(LED_GREEN_FLASHMS);
		// digitalWrite(LED_GREEN, LOW);
		// //threads.delay(LED_GREEN_FLASHMS);
		// threads.yield();
    //incr *= -1;
    
    unsigned long endTime = micros();
    unsigned long elapsedTime = endTime - startTime; // calculate the elapsed time in microseconds
    delay(0.3);
    threads.yield();

   // Serial.print("f2\n");


 // Serial.print("Elapsed time: ");
 // Serial.print(elapsedTime);
 // Serial.print(" microseconds");
  //Serial.print(1-(elapsedTime/1000000));
 // Serial.print(incr);
  //Serial.print("\n"); 
	}
}

void setup() {
	// Setup LEDs
	//pinMode(LED_BLUE, OUTPUT);
//	pinMode(LED_GREEN, OUTPUT);
//	pinMode(LED_RED, OUTPUT);
	// Add Single Thread for Each LED
	threads.addThread(f1);
	threads.addThread(f2);
	//threads.addThread(blinkBlue);
}

void loop() {
	//Serial.printf("hi"); // Nothing to do as all handled in seperate Threads....
}