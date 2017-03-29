#include <AccelStepper.h>

const byte directionPin = 2;
const byte stepPin = 3;
AccelStepper stepper(1,stepPin,directionPin); 

const byte sleepPin = 4;
//const byte resetPin = 5;
const byte enablePin = 5;
const byte ledPin = 13;

void setup()
{  
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(500);

//  DDRD = DDRD | B11111100;  // sets pins 2 to 7 as outputs
//                    // without changing the value of pins 0 & 1 (serial com)
//same as sequence below:                 
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
//  pinMode(resetPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

//  PORTD = PORTD | B00110000;
//  same as below:
  digitalWrite(sleepPin, HIGH);
//  digitalWrite(resetPin, HIGH);
  digitalWrite(enablePin, LOW);
  delay(1); //just in case it was in sleep mode
}

void loop(){   
//  delay(500); 
  stepper.moveTo(100);
  while (stepper.currentPosition() != 50) // Full speed up to 300
    stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
//  // Now stopped after quickstop
    digitalWrite(ledPin, !digitalRead(ledPin));

    digitalWrite(enablePin, HIGH);
    delay(50);
    digitalWrite(enablePin, LOW);
    
  // Now go backwards
  stepper.moveTo(-100);
  while (stepper.currentPosition() != -50) // Full speed basck to 0
    stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
//  // Now stopped after quickstop
    digitalWrite(ledPin, !digitalRead(ledPin));

    digitalWrite(enablePin, HIGH);
    delay(50);
    digitalWrite(enablePin, LOW);
    
  digitalWrite(sleepPin, LOW);
}

