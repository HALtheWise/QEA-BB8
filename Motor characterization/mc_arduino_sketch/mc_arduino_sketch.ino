#include "encoders.h"
#include "motors.h"

float motorspeed = 0.0;

void setup() {
	Serial.begin(57600);
	Serial.println("Motor Characterization test:");
}

void loop() {
	printIfNewEncoder();
	readSerial();
	// moveMotor(motorspeed * 255);
}

const int timeout = 5*1000; // ms
const int heightRead = 24; // encoder ticks
const int heightStop = 48; // encoder ticks

const int holdPower = 0.10*256;
const int returnPower = .06*256;

/* Lifts weight, measuring speed and current and returning ticks/second */
float characterizeMotion(int power) {
	encoder1.write(0);
	long starttime = millis();

	// Start lifting weight
	moveMotor(power);
	while(encoder1.read() < heightRead && (millis() - starttime) < timeout) {}

	// Take the reading
	long readStartTime = millis();

	while(encoder1.read() < heightStop && (millis() - readStartTime) < timeout) {}

	long readStopTime = millis();

	moveMotor(holdPower);

	float dt = (readStopTime - readStartTime)/1000.0;
	float measuredspeed = (heightStop - heightRead) / dt;

	Serial.print("Power: "); Serial.print(power); 
	Serial.print("\tSpeed: "); Serial.println(measuredspeed);

	delay(1000);

	moveMotor(returnPower);

	starttime = millis();
	while(encoder1.read() > 0 && (millis() - readStartTime) < timeout) {}

	moveMotor(0);
}

void readSerial(){
	Serial.setTimeout(20); //20ms timeout on serial reads
	if(Serial.available()){
	    motorspeed = Serial.parseFloat() / 100;
	    Serial.print("Motor speed set to ");
	    Serial.println(motorspeed);
	    delay(3);

	    bool isTest = (Serial.read()=='t');
	    while(Serial.available()){
	    	Serial.read();
	    }

	    if (isTest) {
	    	characterizeMotion(motorspeed * 255);
	    } else {
	    	moveMotor(motorspeed * 255);
	    }
	}
}