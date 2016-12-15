#include <stdlib.h>

#include <Wire.h>

#include "sensor.h"
#include "motors.h"

// Controlling constants

const int LOOP_DURATION = 10; //(ms) This is the inverse of the main loop frequency
const int START_DELAY = 20000;

// Global variable setup (things that change each loop)
long lastActionTime;

// // Setup PID controller
// double PIDerror=0, PIDsetpoint=0, PIDoutput;
// double kp=1,ki=0,kd=0;

int testSpeed = 0;

// PID pid(&PIDerror, &PIDoutput, &PIDsetpoint, kp, ki, kd, DIRECT);

void setup()
{
	Serial.begin(115200);

	lastActionTime = millis();

	setupSensor();

	setupMotors();
}

void loop()
{

	// Every (configurable) milliseconds, average together the readings recieved and transmit them
	long time = millis();
	handleIncomingSerial(); // In the future we might want to read values from serial

	loopSensor(); // sets the new value of ypr. This happens in the sensor.h script

	// Wait until arduino time is greater than (20) seconds before we start running the rest of the main loop.
	// This is because the accelerometer sensor needs about 12-15 seconds to calibrate
	if(millis() < START_DELAY){
	    return;
	}
 

	if (time - lastActionTime >= LOOP_DURATION) {

		// testMotors();
		testSensorsMotors();

		// This formulation attempts to ensure average loop duration is LOOP_DURATION,
		// without causing hyperactive behavior if something blocks for a while.
		lastActionTime = lastActionTime + LOOP_DURATION*int((time-lastActionTime) / LOOP_DURATION);
	}
}

void testMotors(){
	moveMotors(testSpeed);
}

void testSensorsMotors(){
	float theta = ypr[2];

	int power = 0;

	if (theta > 5 * M_PI/180){
		power = 255;
	}
	if (theta < -5 * M_PI/180){
		power = -255;
	}

	Serial.print("power:\t"); Serial.println(power);

	moveMotors(power);
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
