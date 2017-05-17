// Modified from Bounce.pde
// Copyright (C) 2012 Mike McCauley
// Makes a single stepper bounce from one limit to another
#include <AccelStepper.h>

#define stepPin 9
#define directionPin 8
#define enablePin  10

#define motorSpeed 1200 // max 1800
#define motorAccel 10000 // 15000 for speed = 1800

AccelStepper stepper(1,stepPin,directionPin); 

void setup()
{  
  Serial.begin(115200);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);
  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);
  stepper.moveTo(500);
}

void loop()
{
  Serial.println("moving");
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());
      stepper.run();
}
