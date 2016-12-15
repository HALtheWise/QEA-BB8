#include <Arduino.h>

const byte PWM_PIN_A = 9;
const byte PWM_PIN_B = 10;

const byte currentpin = A2;

void setupMotors(){
   pinMode(PWM_PIN_A, OUTPUT);
   pinMode(PWM_PIN_B, OUTPUT);

}

/* 
moveMotor takes a speed in the range -255 ... 255
and drives the motor at that asolute duty cycle
*/
void moveMotors(int speed){
	bool direction = speed > 0;

	if (direction)
	{
		analogWrite(PWM_PIN_A, abs(speed));
		digitalWrite(PWM_PIN_B, LOW);
	} else {
		analogWrite(PWM_PIN_B, abs(speed));
		digitalWrite(PWM_PIN_A, LOW);
	}
}


float calculateMotorPower(float desiredForce) {
	float torque = desiredForce * WHEEL_RADIUS;

	float speed = getEncoderSpeed();

	float power = 0.008905883532020176*speed + 4.723625131842297*(-0.006110361807961874 + torque) + 
   0.06958126214652462*max(-0.19455445221646883,min(0.19455445221646883,speed));

   	return power*255;
}