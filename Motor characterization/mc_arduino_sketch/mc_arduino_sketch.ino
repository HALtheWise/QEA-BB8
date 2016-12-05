#include "encoders.h"
#include "motors.h"

void setup() {
	Serial.begin(9600);
	Serial.println("Basic Encoder Test:");
}

void loop() {
	printIfNewEncoder();
}
