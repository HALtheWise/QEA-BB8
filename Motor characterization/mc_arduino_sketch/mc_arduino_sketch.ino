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

/* Lifts weight, measuring speed and current and returning speed in ticks/sec */
float characterizeMotion(int power) {
	encoder1.write(0);
	long starttime = millis();

	// Start lifting weight
	moveMotor(power);
	while(encoder1.read() < heightRead && (millis() - starttime) < timeout) {}
	if ((millis() - starttime) >= timeout) {
		moveMotor(0);
		return 0;
	}


	// Take the reading
	long readStartTime = millis();

	starttime = millis();
	while(encoder1.read() < heightStop && (millis() - readStartTime) < timeout) {}
	if ((millis() - starttime) >= timeout) {
		moveMotor(0);
		return 0;
	}

	long readStopTime = millis();

	moveMotor(holdPower);

	float dt = (readStopTime - readStartTime)/1000.0;
	float measuredspeed = (heightStop - heightRead) / dt;

	Serial.print("Power: "); Serial.print(power); 
	Serial.print("\tSpeed: "); Serial.println(measuredspeed);

	delay(1000);

	moveMotor(returnPower);

	starttime = millis();
	while(/*encoder1.read() > 0 &&*/ (millis() - readStartTime) < timeout) {}

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
	    } else {
		    Serial.print("Motor speed set to ");
		    Serial.println(motorspeed);
	    	moveMotor(motorspeed * 255);
	    }
	}
}