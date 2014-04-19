/* 
Controlling solenoids with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
Adapted from Adafruit Motorshield v2 - DC Motor test
VP - 4/9/14
----------------
Added IR control - VP - 4/14/2014
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

int IRread = A0;  
//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are also known as A4 and A5
int baseline = 0;
int IRval = baseline;           // store read value


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  LeftSolenoid->setSpeed(0);
  LeftSolenoid->run(FORWARD);
  // turn on current in solenoid coil
  LeftSolenoid->run(RELEASE);
  
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }
  
  // IR sensor initialization
    arrayinit();  
    
}

void loop() {
  
  IRval = analogRead(IRread);    // read the input pin
  Serial.println(IRval);  
  
  if (IRval > (baseline + 30)) {
    // open solenoid
  Serial.print("open");
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(185);  // corresponds to 6V ouput with external power supply 
                              // ~5.5V when solenoid plugged in                              
                           // 3.24V output with USB 
  // opening duration
  delay(2000);
  // refractory period
  Serial.print("close");
  LeftSolenoid->run(RELEASE);
  delay(500);
  }
}

void arrayinit(){
  int  inc;
  int total;
  float IRvalArray[3];
  
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (total!=3){
  total=0;
  for (inc=0; inc < 4 ; inc++ ){
    IRvalArray[inc] = analogRead(IRread);
    if ((inc>0) && IRvalArray[inc]==IRvalArray[inc-1]){
      total++;
    }   
  delay(10);  
  Serial.println((long)IRvalArray[inc], DEC);  
  }
  }
  baseline=IRvalArray[3];
  Serial.print("Baseline value is ");
  Serial.println(baseline);

  Serial.println("End initialization");
}
