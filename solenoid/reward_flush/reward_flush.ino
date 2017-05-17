#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  LeftSolenoid->setSpeed(0);
  LeftSolenoid->run(FORWARD);
  // turn on current in solenoid coil
  LeftSolenoid->run(RELEASE);
  
}

void loop() {
  rewardflush();
  delay(500);
  LeftSolenoid->run(BRAKE);
  LeftSolenoid->run(RELEASE); // cut power to motor
}

void rewardflush(){
  Serial.println("flush");
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(185);
}
