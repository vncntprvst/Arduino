/* 
Whisker platform task
---------------------
Mouse go to front panel, then to either reward port
*/

// intended for use with python GUI (default: arduinoGUI2.py)
// includes a function to send data to the PC

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Left solenoid on port M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);
// Right solenoid on port M2
Adafruit_DCMotor *RightSolenoid = AFMS.getMotor(2);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *TexturePanelStepper = AFMS.getStepper(200, 2);

const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period

//const int TTLPin = 12;
const int SoundTriggerPin=11;

int curr_pos=0; // panel position, 0 or 1
int next_pos;
int rot_seq; // number of panel rotations


//=============

void setup() {
 Serial.begin(9600);
 randomSeed(analogRead(A4));
 
 pinMode(SoundTriggerPin, OUTPUT);
  
 AFMS.begin();  // initiate motor shield with the default frequency 1.6KHz
    
 TexturePanelStepper->setSpeed(30);  // 30 rpm 
}

//=============

void loop() {

      TTLout();

      panelrotate(); // set for next trial 
      
      delay(2000);
}

//=============

void TTLout(){
//HIGH triggers trinkets listening
Serial.println("TTL out");
digitalWrite(SoundTriggerPin, HIGH);
delay(10);
//instructions
digitalWrite(SoundTriggerPin, LOW); // 1
delay(20);
digitalWrite(SoundTriggerPin, HIGH);
delay(10);
digitalWrite(SoundTriggerPin, LOW); // 2
//delay(2);
//digitalWrite(SoundTriggerPin, HIGH);
//delay(2);
//digitalWrite(SoundTriggerPin, LOW); // 3
}

//=============

void panelrotate(){
  // Rotate panel for next trial
  // here random rotation; may be replaced by instruction from computer
  next_pos = random(200); // random number between 0 and 1
//  Serial.print("panelrotate next pos ");
//  Serial.println(next_pos);
  rot_seq = random(200); // random number between 0 and 1
//  Serial.print("panelrotate rot seq ");
//  Serial.println(rot_seq);
  
  if (curr_pos < 100 && next_pos < 100){ //different texture
    if (rot_seq < 100 ) { 
//      Serial.println("Rotate CW 1/2");
//      TexturePanelStepper->step(100, FORWARD, MICROSTEP);
//      delay(100);
      Serial.println("Rotate CCW 3/4");
      TexturePanelStepper->step(150, BACKWARD, DOUBLE);
      } else {
//      Serial.println("Rotate CCW 1/2");
//      TexturePanelStepper->step(100, BACKWARD, MICROSTEP);
//      delay(100);
      Serial.println("Rotate CW 3/4");
      TexturePanelStepper->step(150, FORWARD, DOUBLE);
      }
    } else {
      if (rot_seq < 100 ) {
//      Serial.println("Rotate CW 1/2");
//      TexturePanelStepper->step(100, FORWARD, MICROSTEP);
//      delay(100);
      Serial.println("Rotate CCW 1/2");
      TexturePanelStepper->step(100, BACKWARD, DOUBLE);
      } else {
//      Serial.println("Rotate CCW 1 and 1/2");
//      TexturePanelStepper->step(300, BACKWARD, MICROSTEP);
//      delay(100);
      Serial.println("Rotate CW 1/2");
      TexturePanelStepper->step(100, FORWARD, DOUBLE);
      }
    }
//    TexturePanelStepper->release();
    curr_pos = next_pos;
}


