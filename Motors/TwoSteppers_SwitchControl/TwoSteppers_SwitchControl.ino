#include <AccelStepper.h>
// Stepper 1 controlled through ST-M5045
// Stepper 2 controlled through Polulu 
// Stepper 1
#define stepPinS1 8
#define directionPinS1 9
#define enablePinS1  12
#define buttonCWpin 10
#define buttonCCWpin  11
#define motorSpeed 1800 // max 1800, base 1200 (ST-M5045 set at 400 step per rev, 2 microsteps)
#define motorAccel 10000 // 15000 for speed = 1800, 10000 for 12000
// Stepper 2 
#define stepPinS2 3
#define directionPinS2 2
#define enablePinS2  6
#define sleepPin 4
#define resetPin 5
// other defs
#define ledPin 13
boolean buttonCWpressed = false;
boolean buttonCCWpressed = false;
boolean stimRotation = false;
boolean railMotion = false;
AccelStepper railStepper(1,stepPinS1,directionPinS1); 
AccelStepper stimStepper(1,stepPinS2,directionPinS2); 

void setup()
{  
//  Serial.begin(9600);

  pinMode(directionPinS1, OUTPUT);
  pinMode(stepPinS1, OUTPUT);
  pinMode(enablePinS1, OUTPUT);
  pinMode(buttonCWpin, INPUT);
  pinMode(buttonCCWpin, INPUT);

  railStepper.setMaxSpeed(motorSpeed);
  railStepper.setSpeed(motorSpeed);
  railStepper.setAcceleration(motorAccel);
  digitalWrite(enablePinS1, HIGH); // enable output

  //
  stimStepper.setMaxSpeed(2000);
  stimStepper.setAcceleration(500);

  DDRD = DDRD | B11111100;  // sets pins 2 to 7 as outputs
//                    // without changing the value of pins 0 & 1 (serial com)
//same as sequence below:                 
//  pinMode(directionPin, OUTPUT);
//  pinMode(stepPin, OUTPUT);
//  pinMode(sleepPin, OUTPUT);
//  pinMode(resetPin, OUTPUT);
//  pinMode(enablePin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

  PORTD = PORTD | B00110000;
//  same as below:
//  digitalWrite(sleepPin, HIGH);
//  digitalWrite(resetPin, HIGH);
//  digitalWrite(enablePin, LOW);
    delay(1); //just in case stepper 2 was in sleep mode
}

void loop() { 
 readButtons();

// Serial.print("stimRotation ");
// Serial.print(stimRotation);
// Serial.print(" , railMotion ");
// Serial.println(railMotion);
//Serial.print("buttonCWpressed ");
// Serial.print(buttonCWpressed);
// Serial.print(" , buttonCCWpressed ");
// Serial.println(buttonCCWpressed);
  if ((buttonCCWpressed || buttonCWpressed == true) && (stimRotation == false)) {
    digitalWrite(enablePinS1, HIGH);
    actOnButtons();
  } else { //if ((buttonCCWpressed && buttonCWpressed == false)) { //&& (railMotion == false)) 
//  Serial.println("idle");
    digitalWrite(enablePinS1, LOW);
    rotateStim();
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
 railMotion = true;
 if (buttonCWpressed == true) {
 digitalWrite(directionPinS1, LOW);
 railStepper.setSpeed(motorSpeed);
 railStepper.runSpeed();
 }
 if (buttonCCWpressed == true) {
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



