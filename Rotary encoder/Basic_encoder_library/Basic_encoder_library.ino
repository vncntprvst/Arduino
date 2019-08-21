#include <Encoder.h> // (get library here https://github.com/PaulStoffregen/Encoder)
Encoder myEnc(2, 3);
#define pi 3.14
#define clickPerCycle 400
long oldPosition  = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition / clickPerCycle);
  }
}
