/* 
Texture discrimination task
---------------------
Mouse go to front panel, touch textur panel with whiskers, then  go to left or right reward port accordingly.
Rule is: Texture -> go Left; No Texture -> go Right;
Based on Training2_Front_then_Side_Ports_PCcontrol_2FrontLEDs
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

  if(SessionStatus[1] == 1){ // reset counters
//    arrayinit();
    TrialCount=0;
    RewCount=0;
    Lrewtrig=0;
    Rrewtrig=0;
    Frewtrig=0;
  }
SessionStatus[0]=1;
//  while (SessionStatus[0] == 0){ 
//    getDataFromPC();
//    while (digitalRead(FlushPress) == LOW){
//      rewardflush();
//    }
//  }

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
    Serial.println("initialize random starting panel position");
        panelrotate();
    Serial.println(TrialType);
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
            Serial.println("Start"); // this will be skiped by serial read
            // Serial.println("Trial number, Trial type, Succes count, Time "); // file header
          }
          sendToPC(TrialCount,int(TrialType),RewCount);
          if (TrialType==1){
            LeftGLight=1;
            Serial.println("TrialType==1");
          } else if (TrialType==2){
            RightGlight=1;
            Serial.println("TrialType==2");
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
          Serial.println("Open Left Solenoid");
          reward(LeftSolenoid,47);
          SoundOut(1); // white noise mask 
          panelrotate(); // set for next trial
          
    //      Serial.print("Reward count: ");
    //      Serial.println(RewCount);
          Lrewtrig=0;
          Rrewtrig=0;
          Frewtrig=0;
          LeftGLight=0;
          RightGlight=0;
          resetfp=1;
          TrialInit=0;
          // refractory period
          delay(2000); // timeout to leave time for 2s white noise
        } else if (Lrewtrig>5 && RightGlight==1){
          if (inPort==0){
            sendToPC(TrialCount,81,0);
            Serial.println("Left Port: incorrect ");
          }
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
            Serial.println("Open Right Solenoid ");
          reward(RightSolenoid,40);
          SoundOut(1); // white noise mask 
          panelrotate(); // set for next trial
    //      Serial.print("Reward count: ");
    //      Serial.println(RewCount);
          Rrewtrig=0;
          Lrewtrig=0;
          Frewtrig=0;
          LeftGLight=0;
          RightGlight=0;
          resetfp=1;
          TrialInit=0;
          // refractory period
          delay(2000);  // timeout to leave time for 2s white noise 
        } else if (Rrewtrig>5 && LeftGLight==1){
          if (inPort==0){
            sendToPC(TrialCount,82,0);
            Serial.println("Right Port: incorrect ");
          }
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

        SoundOut(1); // white noise mask 
        panelrotate(); // set for next trial

        Rrewtrig=0;
        Lrewtrig=0;
        inPort=0;
        Frewtrig=0;
        resetfp=1;
        TrialInit=0;
      }
    } 
  }
}

//=============

void getDataFromPC() {
// receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    char x = Serial.read();
      // the order of these IF clauses is significant

    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============

void parseData() {

    // split the data into its parts
    // assumes the data will be received as (eg) 1,0,2

  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,","); // get the first part
  SessionStatus[0] = atoi(strtokIndx); //  convert to an integer
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  SessionStatus[1] = atoi(strtokIndx);
  
  strtokIndx = strtok(NULL, ","); 
//    if (strtokIndx=="True"){
//    TrialType = 1;
//  } else if (strtokIndx=="False"){
//    TrialType = 2;
// }
  TrialType = atoi(strtokIndx)+1; // unused for the moment
}

//=============

void sendToPC(int TrialCount, int trial_result, int success_trial_count) {
	// if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
	// 	prevReplyToPCmillis += replyToPCinterval;
	// 	int valForPC = curMillis >> 9; // approx half seconds
	// 	Serial.print('<');
	// 	Serial.print(valForPC);
	// 	Serial.print('>');
	// }

  Serial.print(TrialCount);
  Serial.print(",");
  Serial.print(trial_result);
  Serial.print(",");
  Serial.print(success_trial_count);
  Serial.println(",");
}

//=============

void panelrotate(){
  // Rotate panel for next trial
  // here random rotation; may be replaced by instruction from computer
  next_pos = random(1,201); // random number between 0 and 1
  if (next_pos<101){
    TrialType=1;
    Serial.print("panelrotate says TrialType is ");
    Serial.println(TrialType);
  } else {
    TrialType=2;
    Serial.print("panelrotate says TrialType is ");
    Serial.println(TrialType);
  }
//  Serial.print("panelrotate next pos ");
//  Serial.println(next_pos);
  rot_seq = random(200); // random number between 0 and 1
//  Serial.print("panelrotate rot seq ");
//  Serial.println(rot_seq);
  
  if (((curr_pos < 101) && (next_pos < 101)) || ((curr_pos > 100) && (next_pos > 100))){ 
    if (rot_seq < 100 ) { 
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CCW 1/4");
     TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
    } else {
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE); 
    }
  } else { //present different texture
    if (rot_seq < 100 ) {
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
      delay(100);
//      Serial.println("Rotate CCW 1/4");
      TexturePanelStepper->step(50, BACKWARD, DOUBLE); 
    } else {
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
      delay(100);
//      Serial.println("Rotate CW 1/4");
      TexturePanelStepper->step(50, FORWARD, DOUBLE);
    }
  }
//    TexturePanelStepper->release();
  curr_pos = next_pos;
  Serial.print("curr_pos is now ");
  Serial.println(curr_pos);
}

//=============

void TTLout(int instruct){
  digitalWrite(TTLPin, HIGH);
  delay(10);
  digitalWrite(TTLPin, LOW);
  switch (instruct) {
    case 1: 
    // trial initiation: 2 TTL
    delay(10);
    digitalWrite(TTLPin, HIGH);
    delay(10);
    digitalWrite(TTLPin, LOW);
    break;
    case 2:
  // that's it
    break;
  }
}

//=============

void SoundOut(int instruct){
//HIGH triggers trinkets listening
//Serial.println("TTL out");
  switch (instruct) {
    // both cases should total 10ms
    case 1: 
    // white noise
      digitalWrite(SoundTriggerPin, HIGH); // trigger
// NB: 
// if control PlayTone enabled in WhiteNoise_USbeep
// need to add a total of 10ms (or control beep's duration)
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

void reward(Adafruit_DCMotor* solenoid,int dur){
  solenoid->setSpeed(255);  
  solenoid->run(FORWARD);
//    for (int dec=200; dec>170; dec-=decrease) { //max 255
//    solenoid->setSpeed(dec);
//    delay(15);
//    Serial.println(dec);
//   }
 delay(dur);
 solenoid->run(RELEASE); // cut power to motor
}

//=============
// see also runningMedian Class for Arduino: http://playground.arduino.cc/Main/RunningMedian
 void arrayinit(){
  int  inc;
  int Ltotal;
  int Rtotal;
  int F1total;
  int F2total;
  float LeftIRvalArray[4];
  float RightIRvalArray[4];
  float FrontIR1valArray[4];
  float FrontIR2valArray[4];
  Serial.println(" ");
  Serial.println("Start IR initialization");
  while (Ltotal!=3 && Rtotal!=3 && F1total!=3 && F2total!=3){
    Ltotal=0;
    Rtotal=0;
    F1total=0;
    F2total=0;
    for (inc=0; inc < 4 ; inc++ ){
      LeftIRvalArray[inc] = analogRead(LeftIRread);
      RightIRvalArray[inc] = analogRead(RightIRread);
      FrontIR1valArray[inc] = analogRead(FrontIR1read);
      FrontIR2valArray[inc] = analogRead(FrontIR2read);
      if ((inc>0) && LeftIRvalArray[inc]==LeftIRvalArray[inc-1]){
        Ltotal++;
      }   
      if ((inc>0) && RightIRvalArray[inc]==RightIRvalArray[inc-1]){
        Rtotal++;
      }   
      if ((inc>0) && FrontIR1valArray[inc]==FrontIR1valArray[inc-1]){
        F1total++;
      }  
      if ((inc>0) && FrontIR2valArray[inc]==FrontIR2valArray[inc-1]){
        F2total++;
      }  
      delay(10);  
      Serial.print("Left IR val ");
      Serial.print((long)LeftIRvalArray[inc], DEC); 
      Serial.print(" & Right IR val "); 
      Serial.println((long)RightIRvalArray[inc], DEC);
      Serial.print(" & Front IR 1 val "); 
      Serial.print((long)FrontIR1valArray[inc], DEC);
      Serial.print(" & Front IR 2 val "); 
      Serial.println((long)FrontIR2valArray[inc], DEC);
    }
  }
  Lbaseline=LeftIRvalArray[3];
  Rbaseline=RightIRvalArray[3];
  F1baseline=FrontIR1valArray[3];
  F2baseline=FrontIR2valArray[3];
  Serial.print("Left Baseline value is ");
  Serial.print(Lbaseline);
  Serial.print(" and Right Baseline value is ");
  Serial.println(Rbaseline);
  Serial.print(" and Front Baseline values are ");
  Serial.print(F1baseline);
  Serial.print(" and ");
  Serial.println(F2baseline);
  Serial.println("End initialization");
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

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
