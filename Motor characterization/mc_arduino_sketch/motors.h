#include <Arduino.h>

const byte motorAenA = 7;
const byte motorAenB = 8;
const byte motorApwmA = 9;
const byte motorApwmB = 10;

const byte currentpin = A2;

void init_motors(){
   pinMode(motorAenA, OUTPUT);
   pinMode(motorAenB, OUTPUT);
   pinMode(motorApwmA, OUTPUT);
   pinMode(motorApwmB, OUTPUT);

}

/* 
moveMotor takes a speed in the range -255 ... 255
and drives the motor at that asolute duty cycle
*/
void moveMotor(int speed){
	bool direction = speed > 0;

	if (direction)
	{
		analogWrite(motorApwmA, abs(speed));
		digitalWrite(motorApwmB, LOW);
	} else {
		analogWrite(motorApwmB, abs(speed));
		digitalWrite(motorApwmA, LOW);
	}

	digitalWrite(motorAenA, HIGH);
	digitalWrite(motorAenB, HIGH);
}