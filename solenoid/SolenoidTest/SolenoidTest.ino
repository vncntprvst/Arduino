/* 
Controlling solenoids with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
Adapted from Adafruit Motorshield v2 - DC Motor test
VP - 4/9/14
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *RightSolenoid = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - Solenoid test");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  LeftSolenoid->setSpeed(0);
  LeftSolenoid->run(FORWARD);
  // turn on current in solenoid coil
  LeftSolenoid->run(RELEASE);
}

void loop() {
  // open solenoid
  Serial.print("open");
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(185);  // corresponds to 6V ouput with external power supply 
                              // ~5.5V when solenoid plugged in                              
                           // 3.24V output with USB 
  // opening duration
  delay(2000);
  // refractory period
  Serial.print("pause");
  LeftSolenoid->run(RELEASE);
  delay(500);
  
}
