/* 
Whisker platform
----------------
6/18/2014
Two reward ports, flush, etc
4/22/2014
Random panel rotation added 
4/21/2014
First proto: IR trigger opens solenoid and rotates stepper
Adapted from Adafruit Motor Shield v2 documentation
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *LeftSolenoid = AFMS.getMotor(1);
Adafruit_DCMotor *RightSolenoid = AFMS.getMotor(2);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *TexturePanelStepper = AFMS.getStepper(200, 2);

const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period
const int LeftIRread = A0; 
const int RightIRread = A1;  

//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are also known as A4 and A5
int baseline = 0;
int IRval = baseline;           // store read value
int Lbaseline = 0;
int Rbaseline = 0;
int LeftIRval = Lbaseline;           // store read value
int RightIRval = Rbaseline; 
//int PushState = 0;          // pushbutton status
int LeftGLight = 1;
int RightGlight = 1;
int RewCount=0;

int curr_pos=0; // panel position, 0 or 1
int next_pos;
int rot_seq; // number of panel rotations

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  randomSeed(analogRead(A3));
  
  pinMode(FlushPress, INPUT_PULLUP);  
  pinMode(SwitchLed, OUTPUT);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
  
  LeftSolenoid->run(RELEASE);
  RightSolenoid->run(RELEASE);
  
  TexturePanelStepper->setSpeed(20);  // 30 rpm 
  
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }
  
  // IR sensor initialization
    arrayinit();  
}

void loop() {
  
    while (digitalRead(FlushPress) == LOW){
    rewardflush();
  }

  LeftSolenoid->run(RELEASE);    // make sure to close solenoids
  RightSolenoid->run(RELEASE);
  digitalWrite(SwitchLed,LOW);   // and switch off switch LED
  
  // Detection
  LeftIRval = analogRead(LeftIRread);    // read the input pin
  RightIRval = analogRead(RightIRread); 
//  Serial.print("Left ");
//  Serial.print(LeftIRval);  
//  Serial.print(" Right ");
//  Serial.println(RightIRval); 
  
   if ((LeftIRval > (Lbaseline + 30)) && LeftGLight==1){ // Reward
    // open solenoid
  Serial.print("Open Left Solenoid ");
//  LeftSolenoid->run(FORWARD);
//  LeftSolenoid->setSpeed(185);  // corresponds to 6V ouput with external power supply 
                              // ~5.5V when solenoid plugged in                              
                           // 3.24V output with USB 
//    for (int dec=150; dec>55; dec-=10) {
//    LeftSolenoid->setSpeed(dec);
//    delay(flushcycletime);
// }
  
  // opening duration
//  delay(2000);
  Serial.println("Close");
//LeftSolenoid->run(BRAKE); 
  
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  
  // refractory period
  delay(500);
  
  // Rotate panel for next trial
  // here random rotation; may be replaced by instruction from computer
  next_pos = random(200); // random number between 0 and 1
  Serial.println(next_pos);
  rot_seq = random(200); // random number between 0 and 1
  Serial.println(rot_seq);
  
  if (curr_pos < 100 && next_pos < 100){
    if (rot_seq < 100 ) { 
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      } else {
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      }
    } else {
      if (rot_seq < 100 ) {
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
      } else {
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      }
    }
    
    curr_pos = next_pos;
    
  Serial.println("Final delay");  
  delay(400);
  } else if ((RightIRval > (Rbaseline + 30)) && RightGlight==1){
//    // open solenoid
  Serial.print("Open Right Solenoid "); // "Slow" one.
//  RightSolenoid->setSpeed(185);  // 185 (max) corresponds to 6V ouput with external power supply 
                              // ~5.5V when solenoid plugged in                              
                           // 3.24V output with USB 
  RightSolenoid->run(FORWARD);
    for (int dec=150; dec>55; dec-=10) {
    RightSolenoid->setSpeed(dec);
    delay(flushcycletime+4);
 }
 
// opening duration
//  delay(100);
 
//   RightSolenoid->run(BRAKE);
   
//  // refractory period
  Serial.println("close");
  RightSolenoid->run(BRAKE); // cut power to motor
  
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  
  // Switch ports 
//  LeftGLight=1;
//  RightGlight=1;
  
  // refractory period
  delay(500);
  
  // Rotate panel for next trial
  // here random rotation; may be replaced by instruction from computer
  next_pos = random(200); // random number between 0 and 1
  Serial.println(next_pos);
  rot_seq = random(200); // random number between 0 and 1
  Serial.println(rot_seq);
  
  if (curr_pos < 100 && next_pos < 100){
    if (rot_seq < 100 ) { 
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      } else {
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      }
    } else {
      if (rot_seq < 100 ) {
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      Serial.println("Rotate CW");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
      } else {
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      Serial.println("Rotate CCW");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      }
    }
    
    curr_pos = next_pos;
    
  Serial.println("Final delay");  
  delay(400);
  }
  
}

void arrayinit(){
  int  inc;
  int Ltotal;
  int Rtotal;
  float LeftIRvalArray[3];
  float RightIRvalArray[3];
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (Ltotal!=3 && Rtotal!=3){
  Ltotal=0;
  Rtotal=0;
  for (inc=0; inc < 4 ; inc++ ){
    LeftIRvalArray[inc] = analogRead(LeftIRread);
    RightIRvalArray[inc] = analogRead(RightIRread);
    if ((inc>0) && LeftIRvalArray[inc]==LeftIRvalArray[inc-1]){
      Ltotal++;
    }   
    if ((inc>0) && RightIRvalArray[inc]==RightIRvalArray[inc-1]){
      Rtotal++;
    }   
  delay(10);  
  Serial.print("Left IR val ");
  Serial.print((long)LeftIRvalArray[inc], DEC); 
  Serial.print(" & Right IR val "); 
  Serial.println((long)RightIRvalArray[inc], DEC);
  }
  }
  Lbaseline=LeftIRvalArray[3];
  Rbaseline=RightIRvalArray[3];
  Serial.print("Left Baseline value is ");
  Serial.print(Lbaseline);
  Serial.print(" and Right Baseline value is ");
  Serial.println(Rbaseline);
  Serial.println("End initialization");
}

void rewardflush(){
  Serial.println("flush");
  digitalWrite(SwitchLed,HIGH);
  
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(185);
//  delay(500);
//  for (int dec=181; dec>10; dec-=10) { // 5 seems to bethe limit for turning off solenoid
//    LeftSolenoid->setSpeed(dec);
//    delay(200);
// }
 delay(2000);
  LeftSolenoid->run(RELEASE);
//  delay(2000);

  RightSolenoid->run(FORWARD);
  RightSolenoid->setSpeed(185);
//  delay(700);
//    for (int dec=181; dec>0; dec-=10) {
//    RightSolenoid->setSpeed(dec);
//    delay(flushcycletime);
// }
  delay(2000);
  RightSolenoid->run(RELEASE);

}
