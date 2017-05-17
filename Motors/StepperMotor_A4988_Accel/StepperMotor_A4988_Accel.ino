#include <AccelStepper.h>

#define stepsPerRev_SM2 200 // Stepper motor - NEMA-17 - 200 steps/rev, https://www.adafruit.com/product/324
#define panelStepDifferential 40 // 5 stims=> stepsPerRev_SM2/5
int currentTrialType=1; // inital panel position
int nextTrialType;
int rotationAngle;

const byte directionPin = 2;
const byte stepPin = 3;
AccelStepper stepper(1,stepPin,directionPin); 

const byte sleepPin = 4;
//const byte resetPin = 5;
const byte enablePin = 5;
const byte ledPin = 13;

void setup()
{  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  stepper.setMaxSpeed(600);
  stepper.setAcceleration(1000);
//  stepper.setSpeed(500);
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
  for (int iter=1; iter < 5; iter++){
  Serial.print("iteration");
  Serial.print(iter);
//  
//
//      stepper.move(200);
////      while  (abs(stepper.distanceToGo()) != 20){  // Full speed up until close to target
////    //  while (stepper.currentPosition() != targetPos) {
////        stepper.run();
////        }
////        stepper.stop(); // Stop as fast as possible: sets new target
//        stepper.runToPosition();
//
//      delay(250);
//      
//      stepper.move(-200);
////      while  (abs(stepper.distanceToGo()) != 20){  // Full speed up until close to target
////    //  while (stepper.currentPosition() != targetPos) {
////        stepper.run();
////        }
////        stepper.stop(); // Stop as fast as possible: sets new target
//        stepper.runToPosition();
////
//    delay(250);
////

//    stepper.setCurrentPosition(0);
//    stepper.moveTo(50);
//    stepper.runToPosition(); 
//    delay(250);

      nextTrialType = random(1,6);
      Serial.print(" ; trial type ");
      Serial.print(nextTrialType);

      rotationAngle=(panelStepDifferential * (nextTrialType-currentTrialType))-stepsPerRev_SM2;
      if (rotationAngle>-stepsPerRev_SM2){
        rotationAngle=rotationAngle-stepsPerRev_SM2;
      }
      
      Serial.print("; rotation angle ");
      Serial.println(rotationAngle);
      
      rotatepanel(rotationAngle - stepsPerRev_SM2 , 1, 0);
//      delay(1000);
//      rotatepanel(stepsPerRev_SM2, 1, 10);
      delay(1000);
      currentTrialType = nextTrialType;
      
//    rotatepanel(200,1,250);
//    rotatepanel(200,-1,250);
    
//    for (int targetPos=40; targetPos <=200; targetPos = targetPos+40) {
//      stepper.moveTo(targetPos);
//      while  (abs(stepper.distanceToGo()) != 20){  // Full speed up until close to target
//    //  while (stepper.currentPosition() != targetPos) {
//        stepper.run();
//        }
//        stepper.stop(); // Stop as fast as possible: sets new target
//        stepper.runToPosition();
//      delay(250);
//    //  Serial.println(targetPos);
//    }
//    
//    stepper.move(-200);
//    stepper.runToPosition(); 
//    delay(250);
//      
//    // Now go backwards
//    for (int targetPos=160; targetPos >=0; targetPos = targetPos-40) {
//      stepper.moveTo(targetPos);
//      while  (abs(stepper.distanceToGo()) != 20){  // Full speed up until close to target
//    //  while (stepper.currentPosition() != targetPos) {
//        stepper.run();
//        }
//        stepper.stop(); // Stop as fast as possible: sets new target
//        stepper.runToPosition(); 
//      delay(250);
//    //  Serial.println(targetPos);
//    }
  }
  digitalWrite(sleepPin, LOW);
}

void rotatepanel(long relativePos, int rotDirection,  int pause){

  stepper.move(relativePos * rotDirection);
  stepper.runToPosition(); 
  delay(pause);

}

