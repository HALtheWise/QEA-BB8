volatile int desiredTemp;
volatile float desiredSpeed;
volatile int onOff;
volatile int P;
volatile float I;
float currentTemperature = 20;
float filamentDiameter;


byte serialBuffer[256];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:

  readSerialData();
  if (currentTemperature < desiredTemp)
  {
    currentTemperature = currentTemperature + random(3, 6);
  }
  else
  {
    currentTemperature = currentTemperature - random(2, 4);
  }

  filamentDiameter = random(10);

  printSerialData();
}

void printSerialData()
{
  // printSerialData prints the
  Serial.print(currentTemperature); //
  Serial.print(",");
  Serial.println(filamentDiameter); //
}

void readSerialData()
{
  Serial.readBytes(serialBuffer, 256); // read 256 bytes from serial
  desiredTemp = int(serialBuffer[0]); // The values for the desiredTemp should be between 0 - 255 (size of byte)
  desiredSpeed = float(serialBuffer[1]) / 256; // The values for the desiredSpeed should
  onOff = int(serialBuffer[2]); // The values for the desiredSpeed should
  P = int(serialBuffer[3]); // The values for the desiredSpeed should
  I = float(serialBuffer[4]) / 256; // The values for the desiredSpeed should
}
