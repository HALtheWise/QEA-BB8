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