// present then retract stimulus panel
// rotates stimulus between trial. 


#include <AccelStepper.h>
// Stepper 1 controlled through ST-M5045
// Stepper 2 controlled through Polulu 
// Stepper 1: rail
#define stepPin_SM1 9
#define directionPin_SM1 8
#define enablePin_SM1  10
#define buttonFWpin 11
#define buttonBWpin  12
#define motorSpeed 1200 // max 1800, base 1200 (ST-M5045 set at 400 step per rev, 2 microsteps)
#define motorAccel 10000 // 15000 for speed = 1800, 10000 for 12000
AccelStepper railStepper(1,stepPin_SM1,directionPin_SM1); 
// Stepper 2: stim
#define stepPin_SM2 3
#define directionPin_SM2 2
#define sleepPin_SM2 4
#define enablePin_SM2  5
//#define resetPin 5
AccelStepper stimStepper(1,stepPin_SM2,directionPin_SM2); 

// piezo variables
#define piezoPin A0
int baseline = 5;
int piezoVal = 2;           // store read value
int piezoADC;

// other defs
#define ledPin 13
boolean buttonFWpressed = false;
boolean buttonBWpressed = false;
boolean stimRotation = false;
boolean railMotion = false;

void setup()
{  
  Serial.begin(57600);
  pinMode(directionPin_SM1, OUTPUT);
  pinMode(stepPin_SM1, OUTPUT);
  pinMode(enablePin_SM1, OUTPUT);
  pinMode(buttonFWpin, INPUT);
  pinMode(buttonBWpin, INPUT);
  //
  railStepper.setMaxSpeed(motorSpeed);
  railStepper.setSpeed(motorSpeed);
  railStepper.setAcceleration(motorAccel);
  digitalWrite(enablePin_SM1, HIGH); // enable output
  //
//  DDRD = DDRD | B11111100;  // sets pins 2 to 7 as outputs
//                    // without changing the value of pins 0 & 1 (serial com)
//same as sequence below:                 
  pinMode(directionPin_SM2, OUTPUT);
  pinMode(stepPin_SM2, OUTPUT);
  pinMode(sleepPin_SM2, OUTPUT);
//  pinMode(resetPin, OUTPUT);
  pinMode(enablePin_SM2, OUTPUT);
  //
  stimStepper.setMaxSpeed(2000);
  stimStepper.setAcceleration(500);
  //
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

//  PORTD = PORTD | B00010000;
//  same as below:
  digitalWrite(sleepPin_SM2, HIGH);
//  digitalWrite(resetPin, HIGH);
  digitalWrite(enablePin_SM2, LOW);
  delay(1); //just in case stepper 2 was in sleep mode
}

void loop() { 
 piezoADC = cumSumAnalog(); //peakAnalog();
 Serial.println(piezoADC);
 readButtons();
// Serial.print("stimRotation ");
// Serial.print(stimRotation);
// Serial.print(" , railMotion ");
// Serial.println(railMotion);
// Serial.print("buttonFWpressed ");
// Serial.print(buttonFWpressed);
// Serial.print(" , buttonBWpressed ");
// Serial.println(buttonBWpressed);
  if ((buttonBWpressed || buttonFWpressed == true) && (stimRotation == false)) {
    digitalWrite(enablePin_SM1, HIGH);
    moveRail();
  } else { //if ((buttonBWpressed && buttonFWpressed == false)) { //&& (railMotion == false)) 
//  Serial.println("idle");
    digitalWrite(enablePin_SM1, LOW);
//    rotateStim();
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
 digitalWrite(directionPin_SM1, LOW);
 railStepper.setSpeed(motorSpeed);
 railStepper.runSpeed();
 }
 if (buttonBWpressed == true) {
 digitalWrite(directionPin_SM1, HIGH);
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

//  digitalWrite(sleepPin_SM2, LOW);
  stimRotation = false ;
}

int cumSumAnalog() {
// see also runningMedian Class for Arduino: http://playground.arduino.cc/Main/RunningMedian
//  float piezoArray[2] = {analogRead(piezoPin)};
//  int piezoVal;
//  int minMax;
  int cumVal = analogRead(piezoPin); // analogRead(piezoPin);
  for (int inc=0; inc < 9; inc++ ){
//      piezoVal=analogRead(piezoPin);
      cumVal= cumVal + analogRead(piezoPin);
//      if (piezoArray[0] > piezoVal) {
//        piezoArray[0]=piezoVal;
//        }
//      peakVal = max(peakVal, piezoVal);
   }
//  minMax=piezoArray[1]-piezoArray[0];
//  Serial.print("peak to peak value is ");
//  Serial.println(peakVal);
  return cumVal; //peakVal;
}

