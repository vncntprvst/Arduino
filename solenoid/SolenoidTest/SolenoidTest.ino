/* 
Testing reward volume
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *RightSolenoid = AFMS.getMotor(2);

int RewCount=0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - Solenoid test");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz

  // turn off current in solenoid coil
  LeftSolenoid->run(RELEASE);
  RightSolenoid->run(RELEASE);
  
  delay(2000);
}

void loop() {
  
    while (RewCount <= 500){
  // 500 trials give about ~1ml (.5 each side), so ~2ul per trial (see picture)
  // Left solenoid
  Serial.println("Open Left Solenoid");
  reward(LeftSolenoid);
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  
  // refractory period
  delay(500);

  // Right solenoid
  Serial.println("Open Right Solenoid ");
  reward(RightSolenoid);
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  
  // refractory period
  delay(500);
    }
}

void reward(Adafruit_DCMotor* solenoid){
  solenoid->setSpeed(255);  // 185 (max) corresponds to 6V ouput with external power supply 
                              // ~5.5V when solenoid plugged in                              
                           // 3.24V output with USB 
  solenoid->run(FORWARD);
    for (int dec=255; dec>225; dec-=10) {
    solenoid->setSpeed(dec);
    delay(15);
    Serial.println(dec);
   }
 solenoid->run(RELEASE); // cut power to motor
}
