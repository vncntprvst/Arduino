

#include <avr/interrupt.h>
#include <LEDproxCoup.h> // Header file needed for Proximity Sensors
#include <Wire.h> // Header file for the Wire for MPR121 Capacitive Touch Sensor
#include "mpr121.h" // Header file for the MPR121 Capacitive Touch Sensor
#include <StopWatch.h> // Header file for the StopWatch command
#define nMaxREXCommands 20

LEDproxCoup LpCObj;
byte REXCommands[nMaxREXCommands] = {3,9,6,9,3,6,3,6,3,9,3,0,6,3,6,6,9,3,9,9};
int REXstart = 26; // Digital 26 send information whether the copper plate is touched to REX
int REXoutput = 24; // Digital 24 sends information whether the trial was a success or failure to REX
int REXcommand = 25; // Digital 25 sends binary command from REX (via breakout board #! port 6, no. 20)
int irqpin = 2; // Digital 2 needed for MPR121 state
byte LEDCommand=0x00; // ****What does this do exactly?
byte sensorState=0xff; // ****What does this do exactly? 
boolean touchStates[0]; // Measures the "touch state" of the capacitive touch sensor
StopWatch myStopWatch; // Creates the StopWatch function for Timer Functions

volatile bool isTask, isResult, isNotReady, isActive;
// isTask is true when a trial is currently happening
// isResult ***** What does is result mean?
// isNotReady is true when the hand is not on the starting plate.
// isActive is true when the LED (*** what part of the LED?) is on.

void setup()
{
  Serial.begin(9600);

  // For Proximity Sensors
  LpCObj.Init(); // ***** What does this do exactly? 

  // For MPR121 Capacitive Touch Sensor
  Wire.begin();
  mpr121_setup();
  pinMode(irqpin, INPUT); // Establishes that the irqpin (2) is strictly functions as an input
  digitalWrite(irqpin, HIGH); // Sets the irqpin (2) to high voltage such as 3.3V; it is pulled LOW by a touch

  // For Interfacing with REX
  pinMode(REXcommand, INPUT); // See above comments
  pinMode(REXstart, OUTPUT);
  digitalWrite(REXstart, LOW);
  pinMode(REXoutput, OUTPUT);
  digitalWrite(REXoutput, LOW);
}

void loop()
{
  // Read from Breakout Board and Light Respective LED
  REXcommand = PINA & 15; // Should this be 15 or 25?
  if ((REXcommand != 0))
  // if (receivedREXcommand())
  {
    // Reset paramaters for functioning and testing for a new trial
    isTask = true;
    isResult = true;
    isNotReady = true;
    isActive = false; 

    // Reset inputs for LED board for a new trial 
    LEDCommand = 0x00;
    sensorState = 0xff;
    
    while (isNotReady) {
      isNotReady = readTouchInputs(); // Returns false if the starting plate has been touched 
    }
    
    // Get REXCommand
    LEDCommand = LpCObj.getState(REXcommand);
    // **** It would be a good idea to look at the LEDproxCoup header file to understand this
    // because I don't know if the getState input is correct ...

    // Also consider how you will check to make sure that the monkey does not use two hands 
    // or that there is a check to see that once the task has started the monkey should move 
    // his or her hand off of the plate.
    
    // Start the timer for the task
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    
    // Enters and Waits for Timer 1 to Fire
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
    // ^^^ Should this be commented? I think I inherited this ...
    
    PORTF = 0x00;    


    if (LpCObj.checkProx(LEDCommand,sensorState)) 
    {
      PORTF = LpCObj.getState(REXCommands[iCommand] - 1); // Outputs Green to PORTF
      digitalWrite(REXoutput, HIGH); // Designates a success ... HIGH = true = 1
    }
    else {
      PORTF = LpCObj.getState(REXCommands[iCommand] - 2); // Outputs Red to PORTF
      digitalWrite(REXoutput, LOW); // Designates a failure ... LOW = false = 0
    }
      
    // Start Timer for Result
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    while(isResult);
    // ***** ^^^^ This might also be unnecessary because we don't need a time to designate
    // how long this needs to be ....

    PORTF = 0x00; // I'm getting the feeling that PORTF is the LED light. 
    //stopTimer after Result
    //TCCR1B = 0x00;
    
  } else {
    digitalWrite(REXstart, HIGH);
    digitalWrite(REXoutput, HIGH);
  }
}

boolean receivedREXcommand (void){
	return digitalRead(REXcommand); // will return true (1, HIGH) or false (0, LOW)
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
        
        } else if(touchStates[i] == 1){
          // pin i is still being touched
          // if (!myStopWatch.isRunning())
            // myStopWatch.start();
          // If pin is held longer than 0.5 second
          if (myStopWatch.elapsed() > 500) {
            digitalWrite(REXstart, HIGH);
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
    digitalWrite(REXstart, HIGH);
    return true;
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

