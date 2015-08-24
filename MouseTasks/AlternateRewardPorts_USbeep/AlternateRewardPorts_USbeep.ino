/* 
Alternate between left and right ports - VP - 6/5/2014
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


const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period
const int LeftIRread = A0; 
const int RightIRread = A1;  
const int FrontIRread = A2;  

//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are A4 and A5
int Lbaseline = 0;
int Rbaseline = 0;
int Fbaseline = 0;
int LeftIRval = Lbaseline;           // store read value
int RightIRval = Rbaseline; 
int FrontIRval = Rbaseline; 
int LeftGLight = 1; // Good to go ("green light")
int RightGlight = 1;
//int PushState = 0;          // pushbutton status
int RewCount=0;
int Lrewtrig=0;
int Rrewtrig=0;

// sound stimulus
const int SoundTriggerPin=11;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  randomSeed(analogRead(A4));
  pinMode(FlushPress, INPUT_PULLUP);  
  pinMode(SwitchLed, OUTPUT);
  pinMode(SoundTriggerPin, OUTPUT);
  
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
  
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }
  
  // IR sensor initialization
    arrayinit();  
    
}

void loop() {
//  PushState= digitalRead(FlushPress);
//  Serial.print("PushState = ");  
//  Serial.println(PushState);  
  // check for flush activation 
  
  while (digitalRead(FlushPress) == LOW){
    rewardflush();
  }
  
  LeftSolenoid->run(RELEASE);    // make sure to close solenoids
  RightSolenoid->run(RELEASE);
  digitalWrite(SwitchLed,LOW);   // and switch off switch LED
  
  LeftIRval = analogRead(LeftIRread);    // read the input pin
  RightIRval = analogRead(RightIRread); 
//  Serial.print("Left ");
//  Serial.print(LeftIRval);  
//  Serial.print(" Right ");
//  Serial.println(RightIRval);  
 FrontIRval = analogRead(FrontIRread);
  //  Serial.print("Front IR ");
//  Serial.print(FrontIRval);

 if ((LeftIRval > (Lbaseline + 300)) && LeftGLight==1){
    Lrewtrig=Lrewtrig+1;
  }else if ((RightIRval > (Rbaseline + 300)) && RightGlight==1){
    Rrewtrig=Rrewtrig+1;
  }
  
    if (Lrewtrig>5){
    TTLout(2); // for reward US
    //delay(110); // delay before next sound needs to be higher than bleep duration
    // open solenoid
  Serial.println("Open Left Solenoid");
  reward(LeftSolenoid);
//  panelrotate();
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  Lrewtrig=0;
   
  // Switch ports 
  LeftGLight=0;
  RightGlight=1;
  
  // refractory period
  delay(1000);
  } 
 
 if (Rrewtrig>5){
   TTLout(2); // for reward US
   //delay(110); // delay before next sound needs to be higher than bleep duration
  // Right solenoid
  Serial.println("Open Right Solenoid ");
  reward(RightSolenoid);
//  panelrotate();
  RewCount=RewCount+1;
  Serial.print("Reward count: ");
  Serial.println(RewCount);
  Rrewtrig=0;
    
  // Switch ports 
  LeftGLight=1;
  RightGlight=0;
  
  // refractory period
  delay(1000);
  }
}

//=============

void TTLout(int instruct){
//HIGH triggers trinkets listening
Serial.println("TTL out");
switch (instruct) {
    // both cases should total 10ms
    case 1: 
    // white noise
      digitalWrite(SoundTriggerPin, HIGH); // trigger
// NB: 
// if control PlayTone enabled in WhiteNoise_USbeep
// need to add a total of 100ms (or control beep's duration)
// to wait for if statment 
      delay(2); 
      digitalWrite(SoundTriggerPin, LOW); // White noise
      delay(8);
      break;
    case 2:
      // US
      digitalWrite(SoundTriggerPin, HIGH); // trigger
      delay(2); 
      digitalWrite(SoundTriggerPin, LOW);
      delay(2);
      digitalWrite(SoundTriggerPin, HIGH); // +1
      delay(2);
      digitalWrite(SoundTriggerPin, LOW);
      delay(4);
      break;
  }
}

//=============

void arrayinit(){
  int  inc;
  int Ltotal;
  int Rtotal;
  int Ftotal;
  float LeftIRvalArray[4];
  float RightIRvalArray[4];
  float FrontIRvalArray[4];
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


