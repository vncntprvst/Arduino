
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

int inputPin = 30;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *RightSolenoid = AFMS.getMotor(2);

int RewCount=0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - Solenoid test");
  pinMode(inputPin, INPUT);
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz

  // turn off current in solenoid coil
  LeftSolenoid->run(RELEASE);
//  RightSolenoid->run(RELEASE);
  
  delay(2000);
}

void loop() {
  if  (digitalRead(inputPin) == HIGH){
    reward(LeftSolenoid,40);
    RewCount=RewCount+1;
    Serial.print("Reward count: ");
    Serial.println(RewCount);
//    refractory period
//    delay(500);
    }
}

void reward(Adafruit_DCMotor* solenoid,int dur){
  solenoid->setSpeed(255);  
  solenoid->run(FORWARD);
//    for (int dec=200; dec>170; dec-=decrease) { //max 255
//    solenoid->setSpeed(dec);
//    delay(15);
//    Serial.println(dec);
//   }
 delay(dur);
 solenoid->run(RELEASE); // cut power to motor
}
