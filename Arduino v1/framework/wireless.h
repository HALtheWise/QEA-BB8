#include <math.h>

#include <SoftwareSerial.h>


SoftwareSerial btSerial(10, 11); // RX, TX

void setupWireless(){

	// set the data rate for the SoftwareSerial port
	// NOTE The software serial port can only run up to 19200 bps.
	// The JY-MCU bluetooth unit defaults to 9600 baud, which is used here.
	btSerial.begin(9600);
}

void loopWireless() {
	if (btSerial.available() > 3)
	{
		btSerial.setTimeout(10); // Timeout is 10ms for parseInt(), parseFloat(), and readBytes()
		byte identifier = btSerial.read();
		byte sensorType = btSerial.parseInt();
		float x = btSerial.parseFloat();
		float y = btSerial.parseFloat();
		float z = btSerial.parseFloat();

		while (!btSerial.available());

		byte terminator = btSerial.read();

		if (start == '>' && identifier == '\n' && sensorType == 9){
			Serial.println("good BT message");	
		}else{
			Serial.println("bad BT message");
		}

		while (btSerial.available()){
			btSerial.read();
		}
	}
}