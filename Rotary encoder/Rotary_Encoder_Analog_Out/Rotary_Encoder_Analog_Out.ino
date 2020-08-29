#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
Encoder myEnc(2, 3);
#define pi 3.14
#define clickPerCycle 400
long oldPosition  = 0;
uint phaseVal;   
//long distanceTraveled = 0;

void setup() {
  pinMode(A14, OUTPUT); 
//  Serial.begin(9600);
}

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    phaseVal = (abs(newPosition) % clickPerCycle)*255/clickPerCycle;
//    Serial.println(phaseVal);
//    distanceTraveled = 2*pi*4*newPosition/clickPerCycle;
//    Serial.println(-2*pi*newPosition/clickPerCycle); // multiply by distance from center (e.g.,*4) to convert to actual distance.
//    Serial.println(",");
    analogWrite(A14, phaseVal);
  }
}
