#include <AccelStepper.h>
// Stepper 1 controlled through ST-M5045
// Stepper 2 controlled through Polulu 
// Stepper 1: rail
#define stepPinS1 9
#define directionPinS1 8
#define enablePinS1  10
#define buttonFWpin 11
#define buttonBWpin  12
#define motorSpeed 1800 // max 1800, base 1200 (ST-M5045 set at 400 step per rev, 2 microsteps)
#define motorAccel 10000 // 15000 for speed = 1800, 10000 for 12000
AccelStepper railStepper(1,stepPinS1,directionPinS1); 
// Stepper 2: stim
#define stepPinS2 3
#define directionPinS2 2
#define sleepPin 4
#define enablePinS2  5
//#define resetPin 5
AccelStepper stimStepper(1,stepPinS2,directionPinS2); 
// other defs
#define ledPin 13
boolean buttonFWpressed = false;
boolean buttonBWpressed = false;
boolean stimRotation = false;
boolean railMotion = false;

void setup()
{  
  Serial.begin(115200);
  pinMode(directionPinS1, OUTPUT);
  pinMode(stepPinS1, OUTPUT);
  pinMode(enablePinS1, OUTPUT);
  pinMode(buttonFWpin, INPUT);
  pinMode(buttonBWpin, INPUT);
  //
  railStepper.setMaxSpeed(motorSpeed);
  railStepper.setSpeed(motorSpeed);
  railStepper.setAcceleration(motorAccel);
  digitalWrite(enablePinS1, HIGH); // enable output
  //
//  DDRD = DDRD | B11111100;  // sets pins 2 to 7 as outputs
//                    // without changing the value of pins 0 & 1 (serial com)
//same as sequence below:                 
  pinMode(directionPinS2, OUTPUT);
  pinMode(stepPinS2, OUTPUT);
  pinMode(sleepPin, OUTPUT);
//  pinMode(resetPin, OUTPUT);
  pinMode(enablePinS2, OUTPUT);
  //
  stimStepper.setMaxSpeed(2000);
  stimStepper.setAcceleration(500);
  //
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

//  PORTD = PORTD | B00010000;
//  same as below:
  digitalWrite(sleepPin, HIGH);
//  digitalWrite(resetPin, HIGH);
  digitalWrite(enablePinS2, LOW);
  delay(1); //just in case stepper 2 was in sleep mode
}

void loop() { 
 readButtons();
 Serial.print("stimRotation ");
 Serial.print(stimRotation);
 Serial.print(" , railMotion ");
 Serial.println(railMotion);
 Serial.print("buttonFWpressed ");
 Serial.print(buttonFWpressed);
 Serial.print(" , buttonBWpressed ");
 Serial.println(buttonBWpressed);
  if ((buttonBWpressed || buttonFWpressed == true) && (stimRotation == false)) {
    digitalWrite(enablePinS1, HIGH);
    moveRail();
  } else { //if ((buttonBWpressed && buttonFWpressed == false)) { //&& (railMotion == false)) 
//  Serial.println("idle");
    digitalWrite(enablePinS1, LOW);
    rotateStim();
  }
}

void readButtons() {
 buttonBWpressed = false;
 buttonFWpressed = false;
 
 if (digitalRead(buttonFWpin) == HIGH) {
 buttonFWpressed = true;
// Serial.println("Forward");
 }
 if (digitalRead(buttonBWpin) == HIGH) {
 buttonBWpressed = true;
// Serial.println("Backward");
 }
}

void moveRail() {
 railMotion = true;
 if (buttonFWpressed == true) {
 digitalWrite(directionPinS1, LOW);
 railStepper.setSpeed(motorSpeed);
 railStepper.runSpeed();
 }
 if (buttonBWpressed == true) {
 digitalWrite(directionPinS1, HIGH);
 railStepper.setSpeed(-motorSpeed);
 railStepper.runSpeed();
 }
 railMotion = false;
}

void rotateStim() {
  stimRotation = true ;
  stimStepper.moveTo(100);
  while (stimStepper.currentPosition() != 50) // Full speed up to 300
    stimStepper.run();
    stimStepper.stop(); // Stop as fast as possible: sets new target
    stimStepper.runToPosition(); 
//  // Now stopped after quickstop
    digitalWrite(ledPin, !digitalRead(ledPin));
//delay(500);

  // Now go backwards
  stimStepper.moveTo(-100);
  while (stimStepper.currentPosition() != -50) // Full speed basck to 0
    stimStepper.run();
    stimStepper.stop(); // Stop as fast as possible: sets new target
    stimStepper.runToPosition(); 
//  // Now stopped after quickstop
    digitalWrite(ledPin, !digitalRead(ledPin));

//  digitalWrite(sleepPin, LOW);
  stimRotation = false ;
}



