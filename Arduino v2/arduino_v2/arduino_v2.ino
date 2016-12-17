#include <stdlib.h>

#include <Wire.h>

int testSpeed = 0;

#include "imu.h"
#include "encoders.h"
#include "sensorfusion.h"
#include "motors.h"
#include "controller.h"

// Controlling constants

const int LOOP_DURATION = 10; //(ms) This is the inverse of the main loop frequency
const int START_DELAY = 2000;

// Global variable setup (things that change each loop)
long lastActionTime;

// // Setup PID controller
// double PIDerror=0, PIDsetpoint=0, PIDoutput;
// double kp=1,ki=0,kd=0;


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
	loopEncoder();

	failsafes();

	// Wait until arduino time is greater than (20) seconds before we start running the rest of the main loop.
	// This is because the accelerometer sensor needs about 12-15 seconds to calibrate
	if(millis() < START_DELAY){
		return;
	}


	if (time - lastActionTime >= LOOP_DURATION) {

		// testMotors();
		// testSensorsMotors();

		calculateFusedSensors();
		float motorForce = PDPDcontrol();
		float motorPower = calculateMotorPower(motorForce);
		moveMotors(motorPower);

		// This formulation attempts to ensure average loop duration is LOOP_DURATION,
		// without causing hyperactive behavior if something blocks for a while.
		lastActionTime = lastActionTime + LOOP_DURATION*int((time-lastActionTime) / LOOP_DURATION);

		printDebugInformation(motorForce, motorPower);
	}
}

void failsafes(){
	float encoderSpeed = getEncoderSpeed();
	if(abs(encoderSpeed) > 120){
		moveMotors(0);
		Serial.println("Speed failsafe activated");

		while(true){}
	}
}

const int DEBUG_DURATION = 500; //ms

void printDebugInformation(float force, float power) {
	static int runcount = 0;

	if(runcount == 0){
		Serial.println("theta, thetadot, x, xdot, encoder, encoderdot, motorforce, motorpower");
	}

	if (runcount % (DEBUG_DURATION/LOOP_DURATION) == 0){
		Serial.print(theta);
		Serial.print(", \t");
		Serial.print(thetadot);
		Serial.print(", \t");
		Serial.print(x);
		Serial.print(", \t");
		Serial.print(xdot);
		Serial.print(", \t");
		Serial.print(getEncoderVal());
		Serial.print(", \t");
		Serial.print(getEncoderSpeed());
		Serial.print(", \t");
		Serial.print(force);
		Serial.print(", \t");
		Serial.print(power);
		Serial.println();

	}

	runcount++;
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
