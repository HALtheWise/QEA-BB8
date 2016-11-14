#include <stdlib.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#include <PID_v1.h>

// Controlling constants

const int LOOP_DURATION = 10; //(ms) This is the inverse of the main loop frequency

const byte RIGHT_ENABLE_1	= 4;
const byte RIGHT_ENABLE_2	= 5;
const byte RIGHT_POWER		= 6;

// Global variable setup (things that change each loop)
long lastActionTime;

// // Setup PID controller
// double PIDerror=0, PIDsetpoint=0, PIDoutput;
// double kp=1,ki=0,kd=0;

int testSpeed = 100;

// PID pid(&PIDerror, &PIDoutput, &PIDsetpoint, kp, ki, kd, DIRECT);

void setup()
{
	Serial.begin(9600);

	lastActionTime = millis();

	// Initialize pins
	// Note that the analog sensors don't need initialization
	pinMode(RIGHT_ENABLE_1, OUTPUT);
	pinMode(RIGHT_ENABLE_2, OUTPUT);
	pinMode(RIGHT_POWER, OUTPUT);

	// pid.SetMode(AUTOMATIC);
	// pid.SetSampleTime(LOOP_DURATION);
	// pid.SetOutputLimits(-1, 1);
}

void loop()
{

	// Every (configurable) milliseconds, average together the readings recieved and transmit them
	long time = millis();
	handleIncomingSerial();

	if (time - lastActionTime >= LOOP_DURATION) {

		testMotors();


		// This formulation attempts to ensure average loop duration is LOOP_DURATION,
		// without causing hyperactive behavior if something blocks for a while.
		lastActionTime = lastActionTime + LOOP_DURATION*int((time-lastActionTime) / LOOP_DURATION);
	}
}

void testMotors(){
	driveMotors(testSpeed);
}

void handleIncomingSerial()
{
	if(Serial.available() > 0){

		Serial.setTimeout(100);
		// Read first number from serial stream
		testSpeed = Serial.parseInt();

		// Ingest remainder of serial buffer in case something went wrong
		while(Serial.available()){
		    Serial.read();
		}

		Serial.println(testSpeed);
	}
}

void driveMotors(int power){
	// Inputs leftPower and rightPower vary from -255...255
	// Code in this function is based on https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-dc-motors

	// For each motor, decide whether to run it FORWARD, BACKWARD, (or RELEASE)
	// These are ternary operators, returning FORWARD if power > 0 and backward otherwise.
	if(power > 0){
	    // Go forward
	    digitalWrite(RIGHT_ENABLE_1, HIGH);
	    digitalWrite(RIGHT_ENABLE_2, LOW);
	} else {
		// Go backward
	    digitalWrite(RIGHT_ENABLE_1, LOW);
	    digitalWrite(RIGHT_ENABLE_2, HIGH);
	}
	
	// Set motor speeds
	analogWrite(RIGHT_POWER, abs(power));

}
