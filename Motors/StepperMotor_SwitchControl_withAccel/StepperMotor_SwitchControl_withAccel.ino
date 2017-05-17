#include <AccelStepper.h>

#define stepPin 9
#define directionPin 8
#define buttonCWpin 11
#define buttonCCWpin  12
#define enablePin  10

#define motorSpeed 1200 // max 1800
#define motorAccel 10000 // 15000 for speed = 1800

boolean buttonCWpressed = false;
boolean buttonCCWpressed = false;
AccelStepper stepper(1,stepPin,directionPin); 

void setup()
{  
  Serial.begin(115200);

  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(buttonCWpin, INPUT);
  pinMode(buttonCCWpin, INPUT);

// test rail
//testrailstepper();
//testrailstepper();
//testrailstepper();

// disable output
 digitalWrite(enablePin, LOW);

  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);
  stepper.setCurrentPosition(0);
//  digitalWrite(enablePin, LOW);  
}

void loop() { 
 stepper.run();
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
 Serial.println("CW rotation");
 }
 if (digitalRead(buttonCCWpin) == HIGH) {
 buttonCCWpressed = true;
 Serial.println("CCW rotation");
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

void testrailstepper(){
   delay(200);
    // enable output
  digitalWrite(enablePin, LOW);

// test foward / backward motion
 unsigned long currentTime = millis();
 unsigned long previousTime = currentTime;
 digitalWrite(directionPin, LOW);
 while (currentTime - previousTime < 200) {
   digitalWrite(enablePin, HIGH);
   stepper.setSpeed(motorSpeed);
   stepper.runSpeed();
   currentTime = millis();
 }
// delay(200);
 digitalWrite(enablePin, LOW);
 delay(200);
 currentTime = millis();
 previousTime = currentTime;
 digitalWrite(directionPin, HIGH); 
 while (currentTime - previousTime < 200) {
 digitalWrite(enablePin, HIGH);
 stepper.setSpeed(-motorSpeed);
 stepper.runSpeed();
 currentTime = millis();
 }
// delay(200);

// disable output
 digitalWrite(enablePin, LOW);
}




