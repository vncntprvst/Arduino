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
const int FrontIRread = A2;
const int TTLPin = 12;
unsigned short resetfp = 1; // reset front panel "exploration"'s validation

unsigned long time;

//The shield uses the SDA and SCL i2c pins to control DC and stepper motors. On the Arduino
//UNO these are A4 and A5
int Lbaseline = 0;
int Rbaseline = 0;
int Fbaseline = 0;
int LeftIRval = Lbaseline;           // store read value
int RightIRval = Rbaseline; 
int FrontIRval = Fbaseline;
int LeftGLight = 1; // Good to go ("green light")
int RightGlight = 1;
//int PushState = 0;          // pushbutton status
int RewCount=0;
int Lrewtrig=0;
int Rrewtrig=0;
int Frewtrig=0;

int curr_pos=0; // panel position, 0 or 1
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

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 3500;

//=============

void setup() {
 Serial.begin(9600);
 randomSeed(analogRead(A4));
    
 pinMode(FlushPress, INPUT_PULLUP);  
 pinMode(SwitchLed, OUTPUT);
 pinMode(TTLPin, OUTPUT);
  
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
    arrayinit();
    RewCount=0;
    Lrewtrig=0;
    Rrewtrig=0;
    Frewtrig=0;
  }

  while (SessionStatus[0] == 0){ 
    getDataFromPC();
    while (digitalRead(FlushPress) == LOW){
      rewardflush();
    }
  }

  LeftSolenoid->run(RELEASE);    // make sure to close solenoids
  RightSolenoid->run(RELEASE);
  digitalWrite(SwitchLed,LOW);   // and switch off switch LED
    
  FrontIRval = analogRead(FrontIRread); // read the input pin
  //  Serial.print("Front IR ");
  //  Serial.println(FrontIRval);
  LeftIRval = analogRead(LeftIRread);    
  //  Serial.print(" Left IR ");
  //  Serial.print(LeftIRval);
  RightIRval = analogRead(RightIRread); 
  //  Serial.print(" Right IR ");
  //  Serial.println(RightIRval);

  if (SessionStatus[0] == 1){ 

    if ((FrontIRval > (Fbaseline + 300))){
        Frewtrig=Frewtrig+1;
    }    

    if (Frewtrig>5){
      if (time+1000<millis() && resetfp==1){
      Serial.println("Front panel explored");
        time = millis();
        resetfp=0;
      }
      
      if ((LeftIRval > (Lbaseline + 300)) && LeftGLight==1){
        Lrewtrig=Lrewtrig+1;
      }else if ((RightIRval > (Rbaseline + 300)) && RightGlight==1){
        Rrewtrig=Rrewtrig+1;
      }
      
        if (Lrewtrig>5){
        // open left solenoid
      TTLout();
      Serial.println("Open Left Solenoid");
      reward(LeftSolenoid);
      panelrotate(); // set for next trial
      RewCount=RewCount+1;
      sendToPC(1,RewCount); // result current trial
      Serial.print("Reward count: ");
      Serial.println(RewCount);
      Lrewtrig=0;
      Rrewtrig=0;
      Frewtrig=0;
      resetfp=1;
      // refractory period
      delay(1000);
      } 
     
     if (Rrewtrig>5){
      // Open right solenoid
      TTLout();
      Serial.println("Open Right Solenoid ");
      reward(RightSolenoid);
      panelrotate(); // set for next trial
      RewCount=RewCount+1;
      sendToPC(2,RewCount); // result current trial
      Serial.print("Reward count: ");
      Serial.println(RewCount);
      Rrewtrig=0;
      Lrewtrig=0;
      Frewtrig=0;
      resetfp=1;
      // refractory period
      delay(1000);
      }
      
      if (time+10000<millis()){ // if waits too long, reset
        Frewtrig=0;
        resetfp=1;
      }
      
     } 


    //~ replyToPC();
    
    // In initial version, Arduino sends a number 
    // (roughly the number of half-seconds since RESET) 
    // to the PC every 3.5 seconds and the Python program 
    // checks for and displays any new data every 2 seconds
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
  TrialType = atoi(strtokIndx); 

}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<Session running? ");
    Serial.print(SessionStatus[0]);
    Serial.print(" Reset?  ");
    Serial.print(SessionStatus[1]);
    Serial.print(" Trial type? ");
    Serial.print(TrialType);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
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

//=============

//void moveServo() {
//  if (servoPos != newServoPos) {
//    servoPos = newServoPos;
//    myServo.write(servoPos);
//  }
//}

//=============

void moveStepper() {
  if (TrialType != 0) {
    // servoPos = newServoPos;
    TexturePanelStepper->step(100, BACKWARD, DOUBLE);
  }
}

//=============

void sendToPC(int trial_result, int success_trial_count) {
	// if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
	// 	prevReplyToPCmillis += replyToPCinterval;
	// 	int valForPC = curMillis >> 9; // approx half seconds
	// 	Serial.print('<');
	// 	Serial.print(valForPC);
	// 	Serial.print('>');
	// }
  Serial.print('<');
  Serial.print(trial_result);
  Serial.print(',');
  Serial.print(success_trial_count);
  Serial.print('>');
}

//=============

void reward(Adafruit_DCMotor* solenoid){
  solenoid->setSpeed(255); 
  solenoid->run(FORWARD);
    for (int dec=255; dec>225; dec-=10) { //that will be ~2.5ul with gravity feed ~20cm above
    solenoid->setSpeed(dec);
    delay(15);
//    Serial.println(dec);
   }
 solenoid->run(RELEASE); // cut power to motor
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
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void TTLout(){
//Serial.println("UP");
digitalWrite(TTLPin, HIGH);
delay(200);
//Serial.println("DOWN");
digitalWrite(TTLPin, LOW);
//delay(100);
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
