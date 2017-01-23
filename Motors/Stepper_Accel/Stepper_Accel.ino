#include <AccelStepper.h>

byte stepPin = 8;
byte directionPin = 9;
AccelStepper stepper(1,stepPin,directionPin); 

void setup()
{  
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(500);
}

void loop(){   
//  delay(500); 
  stepper.moveTo(100);
  while (stepper.currentPosition() != 50) // Full speed up to 300
    stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
//  // Now stopped after quickstop

//delay(500);

  // Now go backwards
  stepper.moveTo(-100);
  while (stepper.currentPosition() != -50) // Full speed basck to 0
    stepper.run();
    stepper.stop(); // Stop as fast as possible: sets new target
    stepper.runToPosition(); 
//  // Now stopped after quickstop
}

