/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <Arduino.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder encoder1(5, 6);
//   avoid using pins with LEDs attached

long lastEncoderPosition = 0;
long lastEncoderDeltaPosition = 0;
unsigned long lastEncoderChangeTime = micros();
unsigned long lastEncoderDeltaTime = 1; // microseconds

const int ENCODER_TIMEOUT = 500*1000; // microseconds, speed is zero if no reading in this time.

// Returns position in ticks
float getEncoderVal(){
	return encoder1.read();
}

// Returns speed in ticks / second
float getEncoderSpeed(){
	unsigned long time = micros();
	if (time - lastEncoderChangeTime >= ENCODER_TIMEOUT)
	{
		return 0.0;
	}

	return (float(lastEncoderDeltaPosition) / lastEncoderDeltaTime) * 1e6;
}

void loopEncoder(){
	long val = encoder1.read();
	if (val != lastEncoderPosition)
	{
		unsigned long time = micros();
		lastEncoderDeltaTime = time - lastEncoderChangeTime;
		lastEncoderDeltaPosition = lastEncoderPosition - val;

		lastEncoderChangeTime = time;
		lastEncoderPosition = val;
	}
}

bool printIfNewEncoder(){
	static long oldPosition  = -999;


	long newPosition = encoder1.read();
	if (newPosition != oldPosition) {
		oldPosition = newPosition;
		Serial.println(newPosition);
		return true;
	}
	return false;
}