// intended for use with arduinoGUI2.py
//   this includes a function to send data to the PC

//#include <Servo.h>
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
byte servoMin = 10;
//byte servoMax = 170;
byte servoPos = 0;
byte newServoPos = servoMin;

const int FlushPress = 2;     // pushbutton pin
const int SwitchLed = 3;     // pushbutton led pin
const int flushcycletime = 20; // solenoid turn off staircase period
const int LeftIRread = A0; 
const int RightIRread = A1;  
const int FrontIRread = A2;
const int TTLPin = 12;
unsigned short resetfp = 1;

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

const byte numLEDs = 2;
byte ledPin[numLEDs] = {12, 13};
byte ledStatus[numLEDs] = {0, 0};

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
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
  
// turn off current in solenoid coil
  LeftSolenoid->run(RELEASE);
  RightSolenoid->run(RELEASE);
    
  TexturePanelStepper->setSpeed(30);  // 10 rpm 
//for(i=0;i<4;i++){ motors[i].run(RELEASE); }
  
  
    // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  curMillis = millis();
  getDataFromPC();
//  switchLEDs();
//  moveServo();
moveStepper();
  //~ replyToPC();
  sendToPC();
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
    // assumes the data will be received as (eg) 0,1,35
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,","); // get the first part
  ledStatus[0] = atoi(strtokIndx); //  convert to an integer
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  ledStatus[1] = atoi(strtokIndx);
  
  strtokIndx = strtok(NULL, ","); 
  newServoPos = atoi(strtokIndx); 

}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<LedA ");
    Serial.print(ledStatus[0]);
    Serial.print(" LedB ");
    Serial.print(ledStatus[1]);
    Serial.print(" SrvPos ");
    Serial.print(newServoPos);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

//=============

void switchLEDs() {

  for (byte n = 0; n < numLEDs; n++) {
//    digitalWrite( ledPin[n], ledStatus[n]);
  }
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
  if (servoPos != newServoPos) {
    servoPos = newServoPos;
    TexturePanelStepper->step(100, BACKWARD, DOUBLE);
  }
}


//=============

void sendToPC() {
	if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
		prevReplyToPCmillis += replyToPCinterval;
		int valForPC = curMillis >> 9; // approx half seconds
		Serial.print('<');
		Serial.print(valForPC);
		Serial.print('>');
	}
	
}
