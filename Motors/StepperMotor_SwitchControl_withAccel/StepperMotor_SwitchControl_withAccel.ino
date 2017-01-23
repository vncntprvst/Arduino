#include <AccelStepper.h>

#define stepPin 8
#define directionPin 9
#define buttonCWpin 10
#define buttonCCWpin  11
#define enablePin  12

#define motorSpeed 1200 // max 1800
#define motorAccel 10000 // 15000 for speed = 1800

boolean buttonCWpressed = false;
boolean buttonCCWpressed = false;
AccelStepper stepper(1,stepPin,directionPin); 

void setup()
{  
//  Serial.begin(9600);

  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(buttonCWpin, INPUT);
  pinMode(buttonCCWpin, INPUT);

  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);

  // enable output
  digitalWrite(enablePin, HIGH);
}

void loop() { 
 readButtons();
  if (buttonCCWpressed || buttonCWpressed == true) {
    digitalWrite(enablePin, HIGH);
    actOnButtons();
  } else {
//  Serial.println("idle");
    digitalWrite(enablePin, LOW);
  }
}

void readButtons() {
 buttonCCWpressed = false;
 buttonCWpressed = false;
 
 if (digitalRead(buttonCWpin) == HIGH) {
 buttonCWpressed = true;
// Serial.println("CW rotation");
 }
 if (digitalRead(buttonCCWpin) == HIGH) {
 buttonCCWpressed = true;
// Serial.println("CCW rotation");
 }
}

void actOnButtons() {
 if (buttonCWpressed == true) {
 digitalWrite(directionPin, LOW);
 stepper.setSpeed(motorSpeed);
 stepper.runSpeed();
 }
 if (buttonCCWpressed == true) {
 digitalWrite(directionPin, HIGH);
 stepper.setSpeed(-motorSpeed);
 stepper.runSpeed();
 }
}

//Try sending a logical 0 to EN input on driver, to see if that releases the stepper
//void sfdggdfhd() {
//
//}



