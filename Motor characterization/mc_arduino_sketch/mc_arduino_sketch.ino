#include "encoders.h"
#include "motors.h"

float characterizeMotion(int power, bool goingUp = true);

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

int holdPower = 0;
int returnPower = 0;

const float maxSpeed = 100;

void manyCharacterize() {

	int minPower = 0;
	for(int power=0; power<256; power += 10){
	    float result = characterizeMotion(power);
	    if (result > 0) {
	    	minPower = power;
	    	moveMotor(0);
	    	break;
	    }
	}
	Serial.print("Discovered lower bound power: ");
	Serial.println(minPower);

	holdPower = minPower - 10;
	returnPower = minPower - 30;


	for(int power=minPower; power<256; power += 10){
	    float result = characterizeMotion(power);
	    if (result > maxSpeed) {
	    	break;
	    }
	}
}

void raiseSlowly(int distance = 200, float raiseSpeed = 30) {
	const float kp = 10;
	long startEncoder = encoder1.read();
	long starttime = millis();

	while(millis() - starttime < timeout){
		float dt = (millis() - starttime)/1000.0;
	    long posTarget = startEncoder - raiseSpeed * dt;

	   	int power = (posTarget - encoder1.read()) * kp;
	   	// power = min(power, maxPower);

	   	moveMotor(power);

	   	if(abs(posTarget - encoder1.read()) > 30) {
	   		break;
	   	}

	   	if(encoder1.read() > distance+startEncoder){
	   	    break;
	   	}
	}
	moveMotor(0);
}

void lowerSlowly(float lowerSpeed = 50, int minPower = 0) {
	const float kp = 10;
	long startEncoder = encoder1.read();
	long starttime = millis();

	while(millis() - starttime < timeout){
		float dt = (millis() - starttime)/1000.0;
	    long posTarget = startEncoder - lowerSpeed * dt;

	   	int power = (posTarget - encoder1.read()) * kp;
	   	power = max(power, minPower);

	   	moveMotor(power);

	   	if(abs(posTarget - encoder1.read()) > 30) {
	   		break;
	   	}
	}
	moveMotor(0);
}

/* Lifts weight, measuring speed and current and returning speed in ticks/sec */
float characterizeMotion(int power, bool goingUp = true) {

	if(!goingUp){
	    raiseSlowly(heightStop);
	}

	encoder1.write(0);
	long starttime = millis();

	// Start lifting/lowering weight
	moveMotor(power);

	if(goingUp){
		while(encoder1.read() < heightRead && (millis() - starttime) < timeout) {}
	} else {  
		while(encoder1.read() > -heightRead && (millis() - starttime) < timeout) {}
	}

	if ((millis() - starttime) >= timeout) {
		lowerSlowly();
		return 0;
	}


	// Take the reading
	long readStartTime = millis();

	starttime = millis();

	long readStartDist = encoder1.read();

	long totalCurrent = 0;
	long loopCount = 0;

	if(goingUp){
		while(encoder1.read() < heightStop && (millis() - starttime) < timeout) {
			loopCount++;
			totalCurrent += analogRead(currentpin);
			delay(1);
		}
	} else {  
		while(encoder1.read() > -heightStop + 24 && (millis() - starttime) < timeout) {
			loopCount++;
			totalCurrent += analogRead(currentpin);
			delay(1);
		}
	}

	long readStopDist = encoder1.read();
	long readStopTime = millis();

	if ((millis() - starttime) >= timeout) {
		lowerSlowly();
		return 0;
	}

	if(goingUp){
		// Decelerate the motor
		moveMotor(power*0.8);
		delay(100);
		moveMotor(power*0.6);
		delay(100);
	}

	float dt = (readStopTime - readStartTime)/1000.0;
	float measuredspeed = (readStopDist - readStartDist) / dt;

	Serial.print("Power: "); Serial.print(power); 
	Serial.print("\tSpeed: "); Serial.println(measuredspeed);

	// Lower the motor
	lowerSlowly();

	moveMotor(0);
	return measuredspeed;
}

void readSerial(){
	Serial.setTimeout(20); //20ms timeout on serial reads
	if(Serial.available()){
	    motorspeed = Serial.parseFloat() / 100;
	    delay(3);

	    byte command = Serial.read();
	    while(Serial.available()){
	    	Serial.read();
	    }

	    if (command=='t') {
		    Serial.print("Test running at power ");
		    Serial.println(motorspeed);
	    	characterizeMotion(motorspeed * 255);
	    } else if(command=='s'){
		    Serial.println("Sweeping");
	       	manyCharacterize();
	    } else if(command=='l'){
		    Serial.println("Lowering");
	       	lowerSlowly();
	    } else {
		    Serial.print("Motor speed set to ");
		    Serial.println(motorspeed);
	    	moveMotor(motorspeed * 255);
	    }
	}
}