#include <Encoder.h>
Encoder myEnc(2, 3);
#define pi 3.14
#define clickPerCycle 400
long oldPosition  = 0;
//long distanceTraveled = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
//    distanceTraveled = 2*pi*4*newPosition/clickPerCycle;
    Serial.print(-2*pi*4*newPosition/clickPerCycle);
    Serial.println(",");
  }
}
