/* 
Texture discrimination task
---------------------
Mouse go to front panel, touch texture panel with whiskers, then go to left or right reward port accordingly.
Rule is: Texture -> go Left; No Texture -> go Right;
Based on TextureDiscriminationTask, with added constraints:
- no reward if going to wrong port 
- [optional] add error audio cue
- timeout after wrong ("false alarm" or "miss") trial: only if texture panel can be removed (otherwise too confusing)
- blocks of trials (e.g., 15 texture/left side reward, then 15 no-texture/right side reward, etc)
*/

// intended for use with Bonsai workflow  (default: RunTask_Video_PerfHisto_HSCam_TimestampedVids.bonsai)
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

//Servo myServo;
//byte servoPin = 8;
//byte servoMin = 10;
//byte servoMax = 170;
//byte servoPos = 0;
//byte newServoPos = servoMin;

const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period
const int LeftIRread = A0; 
const int RightIRread = A1;  
const int FrontIR1read = A2;
const int FrontIR2read = A3;
const int TTLPin = 12;
const int SoundTriggerPin=11; // audio output
unsigned short resetfp = 1; // reset front panel "exploration"'s validation

unsigned long FPtime; // time since front panel exploration
int TrialInit=0;
int TrialSelectMode=0; // 0 -> random selection, 1 -> preset from computer, 2 -> block trial preset (see panel rotate). 
int TrialBlockMode=1; // Turns Block trials mode ON/OFF
int BlockSize=3; // this is the number of trials in each block
int BlockPos=1; // where we are in the block of trials

//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are A4 and A5
int Lbaseline = 0;
int Rbaseline = 0;
int F1baseline = 0;
int F2baseline = 0;
int LeftIRval = Lbaseline;           // store read value
int RightIRval = Rbaseline; 
int FrontIR1val = F1baseline;
int FrontIR2val = F2baseline;
int LeftGLight = 0; // Good to go ("green light")
int RightGlight = 0;
//int PushState = 0;          // pushbutton status
int TrialCount=0;
int RewCount=0;
int Lrewtrig=0;
int Rrewtrig=0;
int Frewtrig=0;
int inPort=0; // keep track of attempts to explore a port 

int curr_pos=0; // inital panel position, curr_pos < 100 means texture side -> go Left (trial type 1)
int next_pos;
int rot_seq; // number of panel rotations

//const byte numLEDs = 2;
//byte ledPin[numLEDs] = {12, 13};

byte SessionStatus[2] = {0, 0}; // 1/ Run [ON/OFF (1/0)] 2/ Reset (1)
byte TrialType = 0; // 0, no trial going on; 1 left trial; 2, right trial // left or right doesn't matter here

// GUI-related variables
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

unsigned long curMillis;

//unsigned long prevReplyToPCmillis = 0;
//unsigned long replyToPCinterval = 3500;

//=============

void setup() {
 Serial.begin(9600);
 randomSeed(analogRead(A4));

if (TrialBlockMode==1){
  TrialSelectMode=2; // not random
}

 pinMode(FlushPress, INPUT_PULLUP);  
 pinMode(SwitchLed, OUTPUT);
 pinMode(TTLPin, OUTPUT);
 pinMode(SoundTriggerPin, OUTPUT);

 AFMS.begin();  // initiate motor shield with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz

// turn off current in solenoid coil
 LeftSolenoid->run(RELEASE);
 RightSolenoid->run(RELEASE);

  TexturePanelStepper->setSpeed(30);  // 30 rpm 
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }

// IR sensor initialization
  arrayinit();   
}

//=============

void loop() {

  // curMillis = millis();
  getDataFromPC();
//    Serial.print("Top of Loop. TrialType = ");
//    Serial.println(TrialType);

  if(SessionStatus[1] == 1){ // reset counters
//    arrayinit();
    TrialCount=0;
    RewCount=0;
    Lrewtrig=0;
    Rrewtrig=0;
    Frewtrig=0;
    TexturePanelStepper->release();
//    curr_pos=0;
  }

//SessionStatus[0]=1;
 while (SessionStatus[0] == 0){
   getDataFromPC();
   while (digitalRead(FlushPress) == LOW){
     rewardflush();
   }
 }

  LeftSolenoid->run(RELEASE);    // make sure to close solenoids
  RightSolenoid->run(RELEASE);
  digitalWrite(SwitchLed,LOW);   // and switch off switch LED

  FrontIR1val = analogRead(FrontIR1read); // read the input pin
  FrontIR2val = analogRead(FrontIR2read); // read the input pin
//  Serial.print("Front IRs ");
//  Serial.print(FrontIR1val);
//  Serial.print(" and ");
//  Serial.println(FrontIR2val);
  
  if (SessionStatus[0] == 1){ 
    if (curr_pos==0){ //initialize random starting panel position
    // Serial.println("initialize random starting panel position");
        panelrotate();
    // Serial.println(TrialType);
    }

    if ((FrontIR1val > (F1baseline + 300)) || (FrontIR2val > (F2baseline + 300))){
      Frewtrig=Frewtrig+1;
    }

    if (Frewtrig>5){
      FPtime=millis();
      resetfp=0;
      while ((FPtime+10000)>millis() && resetfp==0){ // if waits too long, reset

        if (TrialInit==0){ //(FPtime+500)<millis() && 
          TrialCount=TrialCount+1;
          TTLout(1);
          if (TrialCount==1){
            // Serial.println("Start"); // this will be skiped by serial read
            // Serial.println("Trial number, Trial type, Succes count, Time "); // file header
          }
          sendToPC(TrialCount,int(TrialType),RewCount);
          if (TrialType==1){
            LeftGLight=1;
            // Serial.println("TrialType==1");
          } else if (TrialType==2){
            RightGlight=1;
            // Serial.println("TrialType==2");
          }
          TrialInit=1;
          // FPtime = millis();
          SoundOut(2); // for reward US
          delay(100);
        }

        LeftIRval = analogRead(LeftIRread);    
//        Serial.print(" Left IR ");
//        Serial.print(LeftIRval);
        RightIRval = analogRead(RightIRread); 
//        Serial.print(" Right IR ");
//        Serial.println(RightIRval);

        if (LeftIRval > (Lbaseline + 200)){
          Lrewtrig=Lrewtrig+1;
        }else if (RightIRval > (Rbaseline + 200)){
          Rrewtrig=Rrewtrig+1;
        }

        if (Lrewtrig>5 && LeftGLight==1){
          RewCount=RewCount+1;
          TTLout(2);
          sendToPC(TrialCount,1,RewCount); // result current trial
          // open left solenoid
          // Serial.println("Open Left Solenoid");
          reward(LeftSolenoid,47);
          while ((FrontIR1val > (F1baseline + 300)) || (FrontIR2val > (F2baseline + 300))){
          // wait for mouse to get out of front panel
            FrontIR1val = analogRead(FrontIR1read); // read the input pin
            FrontIR2val = analogRead(FrontIR2read);
          }
          trialwrapup(2000);
        } else if (Lrewtrig>5 && RightGlight==1){
          if (inPort==0){
            sendToPC(TrialCount,81,0);
            // Serial.println("Left Port: incorrect - next trial ");
            trialwrapup(3000);
          }
          //code below is now extraneous, but keep it just in case
          inPort=1;
          if ((LeftIRval < (Lbaseline + 200))) {
            inPort=0;
            Lrewtrig=0;
          }
        }

        if (Rrewtrig>5 && RightGlight==1){
          RewCount=RewCount+1;
          TTLout(2);
          sendToPC(TrialCount,2,RewCount); // result current trial
          // Open right solenoid
            // Serial.println("Open Right Solenoid ");
          reward(RightSolenoid,40);
          while ((FrontIR1val > (F1baseline + 300)) || (FrontIR2val > (F2baseline + 300))){
          // wait for mouse to get out of front panel
            FrontIR1val = analogRead(FrontIR1read); // read the input pin
            FrontIR2val = analogRead(FrontIR2read);
          }
          trialwrapup(2000);
        } else if (Rrewtrig>5 && LeftGLight==1){
          if (inPort==0){
            sendToPC(TrialCount,82,0);
            // Serial.println("Right Port: incorrect ");
            trialwrapup(3000);
          }
          //code below is now extraneous (choosing wrong port terminates trial), but keep it just in case
          inPort=1;
          if ((RightIRval < (Rbaseline + 200))) {
            inPort=0;
            Rrewtrig=0;
          }
        }
      }

      delay(1);
      if ((FPtime+10000)<millis() && resetfp==0){
        while ((FrontIR1val > (F1baseline + 300)) || (FrontIR2val > (F2baseline + 300))){
          // wait for mouse to get out of front panel
            FrontIR1val = analogRead(FrontIR1read); // read the input pin
            FrontIR2val = analogRead(FrontIR2read);
        }
          // reset (working version)
        TTLout(2);
        sendToPC(TrialCount,int(90+TrialType),0);
        inPort=0;
        trialwrapup(2000);
      }
    } 
  }
}

