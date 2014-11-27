/* 
Whisker platform
----------------
11/19/2014
Updated code for new Solenoids
11/21/2014
Added front IR sensor
---->	http://www.adafruit.com/products/1438
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


// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *TexturePanelStepper = AFMS.getStepper(200, 2);

const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period
const int LeftIRread = A0; 
const int RightIRread = A1;  
const int FrontIRread = A2;

//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are also known as A4 and A5
int Lbaseline = 0;
int Rbaseline = 0;
int Fbaseline = 0;
int LeftIRval = Lbaseline;           // store read value
int RightIRval = Rbaseline; 
int FrontIRval = Fbaseline;
//int PushState = 0;          // pushbutton status
int LeftGLight = 1;
int RightGlight = 1;
int RewCount=0;
int Lrewtrig=0;
int Rrewtrig=0;

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
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
//  LeftSolenoid->setSpeed(0);
//  RightSolenoid->setSpeed(0);
  // turn on current in solenoid coil
//  LeftSolenoid->run(FORWARD);
//  RightSolenoid->run(FORWARD);
  LeftSolenoid->run(RELEASE);
  RightSolenoid->run(RELEASE);
    
  TexturePanelStepper->setSpeed(60);  // 30 rpm 
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }
  
  // IR sensor initialization
    arrayinit();  
    
}

void loop() {
//  PushState= digitalRead(FlushPress);
//  Serial.print("PushState = ");  
//  Serial.println(PushState);  
  // check for flush activation 
  FrontIRval = analogRead(FrontIRread);
  //Serial.println(FrontIRval);
  while (digitalRead(FlushPress) == LOW){
    rewardflush();
  }
  
  LeftSolenoid->run(RELEASE);    // make sure to close solenoids
  RightSolenoid->run(RELEASE);
  digitalWrite(SwitchLed,LOW);   // and switch off switch LED
  
  LeftIRval = analogRead(LeftIRread);    // read the input pin
  RightIRval = analogRead(RightIRread); 

  if ((LeftIRval > (Lbaseline + 300)) && LeftGLight==1){
    Lrewtrig=Lrewtrig+1;
  }else if ((RightIRval > (Rbaseline + 300)) && RightGlight==1){
    Rrewtrig=Rrewtrig+1;
  }
  
    if (Lrewtrig>5){
    // open solenoid
  Serial.print("Open Left Solenoid");
  reward(LeftSolenoid);
  panelrotate();
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  Lrewtrig=0;
  
  // refractory period
  delay(1000);
  } 
 
 if (Rrewtrig>5){
  // Right solenoid
  Serial.print("Open Right Solenoid "); // "Slow" one.
  reward(RightSolenoid);
  panelrotate();
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  Rrewtrig=0;
  
  // refractory period
  delay(1000);
  }
}



void arrayinit(){
  int  inc;
  int Ltotal;
  int Rtotal;
  int Ftotal;
  float LeftIRvalArray[3];
  float RightIRvalArray[3];
  float FrontIRvalArray[3];
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (Ltotal!=3 && Rtotal!=3 && Ftotal!=3){
  Ltotal=0;
  Rtotal=0;
  Ftotal=0;
  for (inc=0; inc < 4 ; inc++ ){
    LeftIRvalArray[inc] = analogRead(LeftIRread);
    RightIRvalArray[inc] = analogRead(RightIRread);
    FrontIRvalArray[inc] = analogRead(FrontIRread);
    if ((inc>0) && LeftIRvalArray[inc]==LeftIRvalArray[inc-1]){
      Ltotal++;
    }   
    if ((inc>0) && RightIRvalArray[inc]==RightIRvalArray[inc-1]){
      Rtotal++;
    }   
    if ((inc>0) && FrontIRvalArray[inc]==FrontIRvalArray[inc-1]){
      Ftotal++;
    }  
  delay(10);  
  Serial.print("Left IR val ");
  Serial.print((long)LeftIRvalArray[inc], DEC); 
  Serial.print(" & Right IR val "); 
  Serial.println((long)RightIRvalArray[inc], DEC);
  Serial.print(" & Front IR val "); 
  Serial.println((long)FrontIRvalArray[inc], DEC);
  }
  }
  Lbaseline=LeftIRvalArray[3];
  Rbaseline=RightIRvalArray[3];
  Fbaseline=FrontIRvalArray[3];
  Serial.print("Left Baseline value is ");
  Serial.print(Lbaseline);
  Serial.print(" and Right Baseline value is ");
  Serial.println(Rbaseline);
  Serial.print(" and Front Baseline value is ");
  Serial.println(Fbaseline);
  Serial.println("End initialization");
}




void rewardflush(){
  Serial.println("flush");
  // flush left
  LeftSolenoid->run(FORWARD);
  LeftSolenoid->setSpeed(255);
 delay(1500);
  LeftSolenoid->run(RELEASE);
  // flush right
  RightSolenoid->run(FORWARD);
  RightSolenoid->setSpeed(255);
  delay(1500);
  RightSolenoid->run(RELEASE);
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


void panelrotate(){
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
}
