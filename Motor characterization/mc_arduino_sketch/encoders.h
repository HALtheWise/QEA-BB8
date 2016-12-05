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