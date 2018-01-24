#include <avr/interrupt.h>
#include <LEDproxCoup.h>
#include <Wire.h>
#include <mpr121.h>
// or #include "mpr121.h"
#include <StopWatch.h>
#define nMaxREXCommands 20


LEDproxCoup LpCObj;
byte REXCommands[nMaxREXCommands] = {3,9,6,9,3,6,3,6,3,9,3,0,6,3,6,6,9,3,9,9};
byte LEDCommand=0x00;
byte sensorState=0xff;
int irqpin = 2; // Digital 2
boolean touchStates[0];
StopWatch myStopWatch; // Creates the StopWatch function to set a timer

volatile bool isTask, isResult, isNotReady, isActive;

void setup()
{
  LpCObj.Init();
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH);
  // this sets the internal 20K pullup resistor, which will steer the input pin to a HIGH reading unless
  // it is pulled LOW by external circuitry aka a touch? (******for pin configured as an INPUT******)
  Serial.begin(9600);
  Wire.begin();
  
  mpr121_setup();
}

void loop()
{
  for (int iCommand = 0 ; iCommand < nMaxREXCommands ; iCommand++)  
  {
    Serial.print("Test number: ");
    Serial.println(iCommand);
    isTask = true;
    isResult = true;
    isNotReady = true;
    isActive = false;

    
    LEDCommand = 0x00;
    sensorState = 0xff;
    
    //Add a while loop while (true) 
    //true becomes false only when the capacitive sensor is held for longer
    //than a certain amount of time. 
    Serial.println("Reached: Right before while loop for capacitive touch sensing");
    while (isNotReady) {
      isNotReady = readTouchInputs();
    }
    Serial.println("Reached: Out of while loop for CTS");
    
    //getRexCommand
    LEDCommand = LpCObj.getState(REXCommands[iCommand]);
    
    //startTimer for Task
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    
    while(isTask)
    {
      PORTF = LEDCommand;
      if(~isActive)
      {
        sensorState = PINK & 7;
        isActive = (sensorState == 7) ? false : true;
      }
    }
    //stopTimer after Task
    //TCCR1B = 0x00;
    
    PORTF = 0x00;    
    if(LpCObj.checkProx(LEDCommand,sensorState)) {
      PORTF = LpCObj.getState(REXCommands[iCommand] - 1);
      Serial.println("Success");
    }
    else {
      PORTF = LpCObj.getState(REXCommands[iCommand] - 2);
      Serial.println("Failure");
    }
      
    //startTimer for Result
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    while(isResult);
    
    PORTF = 0x00;
    //stopTimer after Result
    //TCCR1B = 0x00;
    
  }
  
}

ISR(TIMER1_COMPA_vect)
{
  if(isTask)  
  {
    isTask = false;  
    isActive = true;
  }
  else
    isResult = false;
  
  TCCR1B = 0x00;
}

boolean readTouchInputs(){
  // Initialize a threshold time constant like 5 seconds
  int timeConstant = 1000;
  if(!checkInterrupt()){ // if irqpin is LOW
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); // request 2 bytes from 0x5A
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    
    int i = 0; // Only using Pin 0, can change so that other pins are used as well....
      if(touched & (1<<i)){
      
        if(touchStates[i] == 0){
          // Start stopwatch
          myStopWatch.start();
          Serial.println("Stopwatch started");
          //pin i was just touched
          Serial.print("pin ");
          Serial.print(i);
          Serial.println(" was just touched");
        
        } else if (touchStates[i] == 1){
          Serial.println("Start?");
          // pin i is still being touched
          // If pin is held longer than 5 seconds
          if (myStopWatch.elapsed() > 1000) {
          myStopWatch.stop();
          myStopWatch.reset();
          return false;
          }
          
        }  
      
        touchStates[i] = 1;      
      } else {
        if(touchStates[i] == 1){
          
          //pin i is no longer being touched
          Serial.print("pin ");
          Serial.print(i);
          Serial.println(" is no longer being touched");
          
          // End the timer
          myStopWatch.stop();
          myStopWatch.reset();
        
          // Hold on! Wait .... 
       }
        
        touchStates[i] = 0;
      }
    
    return true;
  }
}

void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);
 
  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);
  
  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);
  
  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);
  
  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);
  
  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);
  
  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}


boolean checkInterrupt(void){
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}


