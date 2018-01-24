#include "pitches.h"

const int TouchPin= 9;
const int soundPin = 8;
const int outputPin = 7;

void setup() {
    Serial.begin(9600);
    pinMode(TouchPin, INPUT);
    pinMode(soundPin,OUTPUT);
    pinMode(outputPin,OUTPUT);
}

void loop() {
    if(digitalRead(TouchPin)==HIGH)
    {
      digitalWrite(outputPin, HIGH);
      tone(soundPin, NOTE_B3, 100);
      delay(100);
    }
    else
    {
      digitalWrite(outputPin,LOW);
      noTone(soundPin);
    }
}
