/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(30);  // 10 rpm   
}

long timer;
void loop() {
//  Serial.println("Single coil steps");
//  myMotor->step(100, FORWARD, SINGLE); 
//  delay(250);
//  myMotor->step(100, BACKWARD, SINGLE); 
//

//if(
//  Serial.println("Double coil steps");
//  myMotor->step(200, FORWARD, DOUBLE);
//  delay(100);
//  myMotor->step(200, BACKWARD, DOUBLE);
//  
//  
//      Serial.println("Rotate CW 1/4");
//     myMotor->step(50, FORWARD, DOUBLE);
//      delay(100);
////      Serial.println("Rotate CCW 1/4");
//     myMotor->step(50, BACKWARD, DOUBLE); 
//     
//     myMotor->step(50, FORWARD, DOUBLE);
//     delay(100);
//     myMotor->step(50, FORWARD, DOUBLE); 
//
////      Serial.println("Rotate CCW 1/4");
//      myMotor->step(50, BACKWARD, DOUBLE);
//      delay(100);
////      Serial.println("Rotate CW 1/4");
//      myMotor->step(50, FORWARD, DOUBLE); 
//    
//     myMotor->step(50, BACKWARD, DOUBLE);
//     delay(100);
//     myMotor->step(50, BACKWARD, DOUBLE); 
//  
  
 // while(millis()-timer < 250){
  //myMotor->step(0,FORWARD,MICROSTEP);
 // }

//  
//  Serial.println("Interleave coil steps");
//  myMotor->step(100, FORWARD, INTERLEAVE); 
//  myMotor->step(100, BACKWARD, INTERLEAVE); 
//  
//  Serial.println("Microstep steps");
//  myMotor->step(100, FORWARD, MICROSTEP); 
//  delay(250);
//  myMotor->step(150, BACKWARD, MICROSTEP);
    delay(1000);
myMotor->release();
    //timer = millis();
   //while(millis()-timer < 5000){
  //myMotor->step(0,FORWARD,MICROSTEP);
 // }
}
