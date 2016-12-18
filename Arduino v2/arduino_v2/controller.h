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
	const float kPtheta = 10;
	const float kDtheta = 1.4;
	const float kPx = 0.16;
	const float kDx = 0.838;

	const float kEverything = 3.14; // Not pi

	float xset = zeromotion();

	float forceCommand = kPtheta * (theta - 0) + kDtheta * (thetadot) + kPx * (x - xset) + kDx * (xdot);

	forceCommand *= kEverything;
	return forceCommand;
}

float PDcontrol(){
	const float kPtheta = 10;
	const float kDtheta = 1.4;
	const float kPx = 0;
	const float kDx = 0;

	const float kEverything = 3.14; // Not pi

	float xset = zeromotion();

	float forceCommand = kPtheta * (theta - 0) + kDtheta * (thetadot) + kPx * (x - xset) + kDx * (xdot);

	forceCommand *= kEverything;
	return forceCommand;
}

float Constantcontrol(){
	return 2.0; // Newtons
}

float BangBangcontrol(){

	int force = 0;

	if (theta > 5 * M_PI/180){
		force = 2.0;
	}
	if (theta < -5 * M_PI/180){
		force = -2.0;
	}

	// Serial.print("force:\t"); Serial.println(force);

	return force;
}