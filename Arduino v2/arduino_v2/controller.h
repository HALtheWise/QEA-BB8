#include <Arduino.h>

float zeromotion(){
	return 0.0;
}

float constantmotion(){
	const int startTime = 3000;
	const float speed = 1/1000.0;
	if (millis() < startTime)
	{
		return 0;
	}
	return (millis() - startTime) * speed;
}

float PDPDcontrol(){
	const float kPtheta = 1;
	const float kDtheta = -1;
	const float kPx = 1;
	const float kDx = 1;

	float xset = zeromotion();

	float forceCommand = kPtheta * (theta - 0) + kDtheta * (thetadot) + kPx * (x - xset) + kDx * (xdot);
}

void Constantcontrol(){
	moveMotors(testSpeed);
}

void BangBangcontrol(){
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