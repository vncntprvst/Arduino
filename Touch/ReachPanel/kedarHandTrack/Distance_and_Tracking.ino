#define sensorIR1 15              //Must be an analog pin
#define sensorIR2 0

float sensorValue1;               //Must be of type float for pow()
float sensorValue2;  

#include <Wire.h>
#include <PVision.h>

PVision ircam;
byte result;


void setup()

{
  Serial.begin(9600);
  
  ircam.init();
  
}


void loop()
{
  sensorValue1 = analogRead(sensorIR1);
  sensorValue2 = analogRead(sensorIR2);
  
//  Serial.print("Sensor1: ");
//  Serial.print(sensorValue1);
//  Serial.print(" Sensor2: ");
//  Serial.println(sensorValue2);
  
  
  
  result = ircam.read();
  Serial.println(result); 
  if (result & BLOB1)
  {
	Serial.print("BLOB1 detected. X:");
	Serial.print(ircam.Blob1.X);  

	Serial.print(" Y:");
	Serial.print(ircam.Blob1.Y);
	Serial.print(" Size:");
        Serial.println(ircam.Blob1.Size);
  }
 
 
  if (result & BLOB2)
  {
	Serial.print("BLOB2 detected. X:");
	Serial.print(ircam.Blob2.X);
	Serial.print(" Y:");
	Serial.print(ircam.Blob2.Y);
	Serial.print(" Size:");
        Serial.println(ircam.Blob2.Size);
  }
  if (result & BLOB3)
  {
	Serial.print("BLOB3 detected. X:");
	Serial.print(ircam.Blob3.X);
	Serial.print(" Y:");
	Serial.print(ircam.Blob3.Y);
	Serial.print(" Size:");
	Serial.println(ircam.Blob3.Size);
  }
  if (result & BLOB4)
  {
	Serial.print("BLOB4 detected. X:");
	Serial.print(ircam.Blob4.X);
	Serial.print(" Y:");
	Serial.print(ircam.Blob4.Y);
	Serial.print(" Size:");
	Serial.println(ircam.Blob4.Size);
  }

  // Short delay...
  delay(20);
 

}
