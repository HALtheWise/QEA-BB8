#include <Arduino.h>

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