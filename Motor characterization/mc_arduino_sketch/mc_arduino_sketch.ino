#include "encoders.h"
#include "motors.h"

float motorspeed = 0.3;

void setup() {
	Serial.begin(9600);
	Serial.println("Basic Encoder Test:");
}

void loop() {
	printIfNewEncoder();
	readSerial();
	moveMotor(motorspeed * 255);
}

void readSerial(){
	Serial.setTimeout(20); //20ms timeout on serial reads
	if(Serial.available()){
	    motorspeed = Serial.parseFloat() / 100;
	    Serial.print("Motor speed set to ");
	    Serial.println(motorspeed);
	    delay(3);
	    while(Serial.available()){
	    	Serial.read();
	    }
	}
}