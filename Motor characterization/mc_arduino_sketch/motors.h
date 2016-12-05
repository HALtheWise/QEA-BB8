#include <Arduino.h>

const byte motorApinA = 4;
const byte motorApinB = 6;
const byte motorApinSpeed = 5;

/* 
moveMotor takes a speed in the range -255 ... 255
and drives the motor at that asolute duty cycle
*/
void moveMotor(int speed){
	bool direction = speed > 0;

	if (direction)
	{
		digitalWrite(motorApinA, LOW);
		digitalWrite(motorApinB, HIGH);
	} else {
		digitalWrite(motorApinA, HIGH);
		digitalWrite(motorApinB, LOW);
	}

	analogWrite(motorApinSpeed, abs(speed));
}