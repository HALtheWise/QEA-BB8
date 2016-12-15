#include <Arduino.h>
#include <math.h>

const float WHEEL_RADIUS = 0.03302; // meters, equal to 1.3in
const float CYLINDER_RADIUS = .5; // meters

/*
Definition of values:
theta is in degrees, with positive representing head right
encoderdist is in meters, with positive representing head right and body stationary
x is in meters, with positive representing body right

*/

float theta = 0;
float thetadot = 0;
float x = 0;
float xdot = 0;

void calculateFusedSensors(){
	theta = getImuAngle() * (M_PI / 180);
	thetadot = getIMUSpeed() * (M_PI / 180);

	float encoderdist = getEncoderVal() / 12 / (M_2_PI * WHEEL_RADIUS);
	float encoderdistdot = getEncoderSpeed() / 12 / (M_2_PI * WHEEL_RADIUS);

	float distFromTop = (theta / M_2_PI) * (M_2_PI * CYLINDER_RADIUS);
	x = -encoderdist + distFromTop;

	xdot = -encoderdistdot + (thetadot / M_2_PI) * (M_2_PI * CYLINDER_RADIUS);
}