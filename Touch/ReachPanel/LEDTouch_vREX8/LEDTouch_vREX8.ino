    
// 8/27/2013 - VP
// replaced copper plate with IR proximity sensor, to remove starting plate's noise

// 8/13/2013 - VP 
// Put check on starting plate before checking reception of REX command 
// That allows for conditional onset of LED target (i.e., REX running ProcArdCmd(1) )
// The sequence is explained in REX file handeyereachled.d and follow-ups (EHWaitReach ...)

#include <avr/interrupt.h>
#include <LEDproxCoup.h> // Header file needed for Proximity Sensors
//#include <Wire.h> // Header file for the Wire for MPR121 Capacitive Touch Sensor
//#include "mpr121.h" // Header file for the MPR121 Capacitive Touch Sensor
#include <StopWatch.h> // Header file for the StopWatch command

LEDproxCoup LpCObj; // Declare the LEDproxCoup object for flashing lights and sensing hands using proximity sensors
int REXstart = 31; // Digital 31 send information whether the copper plate is touched to REX
int REXoutput = 30; // Digital 30 sends information whether the trial was a success or failure to REX
byte REXCommand;
byte mask = 240; // '00001111' MSB on the right side
//int irqpin = 2; // Digital 2 needed for MPR121 state
int startplate = 13; // That's the proximity sensor replacing the capacitive sensor (copper plate)
unsigned int spval = 1000; // variable used to read starting panel proximity sensor
byte LEDCommand=0x00; // Initialized at 0, sends the color command to which LED
byte sensorState=0xff; // Checks what the value of the proximity sensors are. Initialized at 255
boolean touchStates[0]; // Measures the "touch state" of the touch sensor on starting plate
StopWatch myStopWatch; // Creates the StopWatch function for Timer Functions
// In order to change the timer of the task, change the OCR1A below or in the header file. 

// Troubleshooting
int pin1 = 26;
int pin2 = 27;
int pin4 = 28; 
int pin8 = 29;

int threshold = 4000; // sensitivity

volatile bool isTask, isResult, isNotReady, isActive;
// isTask is true when a trial is currently happening
// isResult is true while the green/red light is on, signifying the success/failure
// isNotReady is true when the hand is not on the starting plate.
// isActive is true when the LED (*** what part of the LED?) is on.

void setup()
{
  Serial.begin(9600);

  // For Proximity Sensors
  LpCObj.Init(); // Initializes the LEDProxCoup class object

  // For MPR121 Capacitive Touch Sensor
//  Wire.begin();
//  mpr121_setup();
//  pinMode(irqpin, INPUT); // Establishes that the irqpin (2) is strictly functions as an input
//  digitalWrite(irqpin, HIGH); // Sets the irqpin (2) to high voltage such as 3.3V; it is pulled LOW by a touch

  // For Interfacing with REX
  pinMode(REXstart, OUTPUT);
  digitalWrite(REXstart, LOW);
  pinMode(REXoutput, OUTPUT);
  digitalWrite(REXoutput, LOW);
  
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin4, INPUT);
  pinMode(pin8, INPUT);
  
//  pinMode(startplate, INPUT);
  
}

void loop()
{
    
    // Reset paramaters for functioning and testing for a new trial
    isTask = false; // Trial has not begun yet
    isResult = true; // I honestly have no idea what this means
    isNotReady = true; // The hand has not been on the copper plate for designated time
    isActive = false; //  The LED is not turned on yet and has not received the REXcommand

    // Reset inputs for LED board for a new trial 
    LEDCommand = 0x00; // No command should be given to the LEDs 
    sensorState = 0xff; // A value of 255 means nothing is touching it, a value of 0 means object nearby
    digitalWrite(REXstart, LOW);  
    digitalWrite(REXoutput, LOW);
    
    // Check whether the hand is on the plate        
    // Serial.print("Before Copper Plate, ... ");
    while (isNotReady) {
      isNotReady = readTouchInputs(); 
    }
    
    // Receive the REX command 
    REXCommand = 0;  // Declare the REXCommand variable
    while ((REXCommand%3 != 0) || (REXCommand == 0)) { // If REXCommand is not 3, 6 or 9 
    REXCommand = PINA & mask; // then keep on reassigning values to REXCommand
    }                
    isTask = true; // Since REXCommand has been received, the trial has started. 
    //Serial.print("Non-shifted REXCommand = ");
    //Serial.println(REXCommand); // Should expect 3 = 48; 6 = 96; 9 = 144
    REXCommand = REXCommand >> 4; // Bitwise Shift by 4 Bits
     
//    Serial.print("REXCommand = ");
//    Serial.println(REXCommand);
//    Serial.print("PinA is = ");
//    Serial.print(digitalRead(pin8));
//    Serial.print(digitalRead(pin4));
//    Serial.print(digitalRead(pin2));
//    Serial.println(digitalRead(pin1));
    
    LEDCommand = LpCObj.getState(REXCommand);
    
    
    // **** It would be a good idea to look at the LEDproxCoup header file to understand this
    // because I don't know if the getState input is correct ...
    // Answer: I believe it is. The header file might be changed so that states 10-15 can be encoded
    // for the 4th and 5th LED. Any specifics that you find troubling? - Frank
    
    // Also consider how you will check to make sure that the monkey does not use two hands 
    // or that there is a check to see that once the task has started the monkey should move 
    // his or her hand off of the plate.
    // Answer: I sort of understand this question, but at the same time, I do not. The LED will
    // turn blue after some designated time and we can only hope that the monkey will move its hand. 
    // I don't think we can program a specific, "Hey, move your hand" or penalization. Unless we don't
    // supply him water for moving his hand, which is not only trickier from a programming standpoint, 
    // but also would probably confuse Rigel. Also, from preventing him from using two hands. Can't we
    // just block the path for one of the hands. - Frank 
    

    // July 15th, 2013 Edit
    OCR1A = 0xFFFF; // Corresponds to a window time of 4.0s 
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Start the Timer for Task
    
    // So we want to check if the proximity sensor has reached 
    int count = 0;
    byte prevSensorState = 0;
    boolean didNotWait = false; // August 14th, 2013: Frank created these two variables didNotWait and REXCommandCheck
    byte REXCommandCheck = 0;
    while (isTask)
    {
      PORTF = LEDCommand; // Lights up the LED for the first time, according to color
      sensorState = PINK & 7; // Read from the proximity sensor and apply 0b00000111
      // So we want a sensorState of 1
//      Serial.print("LEDCommand: ");
//      Serial.println(LEDCommand);
//      Serial.print("sensorState: ");
//      Serial.println(sensorState);
      // isActive = (sensorState == 1); // or isActive = (sensorState == 7) ? false : true;
      // If the monkey's hand reaches the sensor (may have to change so that no noise can trip this)
     
      // Hypothesis 1: Check REXCommand and PINA repeatedly but possibly slows down the while loop ...
//      REXCommandCheck = PINA & mask; // then keep on reassigning values to REXCommand
//      if (REXCommandCheck == 0) { // Because the REX code went to errearltouch b/c early reach
//        didNotWait = true;
//        isTask = false;
//        isActive = true;
//      } else { 
        if (LpCObj.checkProx(LEDCommand,sensorState,prevSensorState))
          count = count + 1;
          else count = 0;
          prevSensorState = sensorState;
          // if (isActive) 
      // } // Part of Hypothesis 1
      if (count > threshold)
      {
        isTask = false;
        isActive = true;
      }
    }
  
    // Hypothesis 2: Check REXCommand and PINA 
//    REXCommandCheck = PINA & mask;
//    if (REXCommandCheck == 0) 
//    didNotWait = true;

    // ***** OLD CODE
    
    // Enters and Waits for Timer 1 to Fire
//    while(isTask)
//    {
//      PORTF = LEDCommand; // Lights up the LED for the first time, according to color. 
//        sensorState = PINK & 7;
//        Serial.println("sensorState is ");
//        Serial.println(sensorState);
//        isActive = (sensorState < 2) ? false : true;
//        // Start the timer for the task
//        if (isActive)
//        TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
//    }
    //stopTimer after Task
    //TCCR1B = 0x00; 
    // ^^^ Should this be commented? I think I inherited this ...
    
    // ***** OLD CODE
    
    PORTF = 0x00;    

//    if (didNotWait == true) {
//      PORTF = LpCObj.getState(REXCommand - 2); // can use get state of 4 or 7
//    } else { // Go through normally
    
    //if (LpCObj.checkProx(LEDCommand,sensorState)) 
    if (count > threshold)
    {
      // PORTF = LpCObj.getState(REXCommand - 1); // Outputs Green to PORTF
      digitalWrite(REXoutput, HIGH); // Designates a success ... HIGH = true = 1
      myStopWatch.stop();
      myStopWatch.reset();
    }
    else {
      // PORTF = LpCObj.getState(REXCommand - 2); // Outputs Red to PORTF
      digitalWrite(REXoutput, LOW); // Designates a failure ... LOW = false = 0
    }
    // }
    
    delay(10);
    REXCommandCheck = PINA & mask;
    if ((digitalRead(REXoutput) == HIGH) && (REXCommandCheck != 0))
      PORTF = LpCObj.getState(REXCommand - 1); // Output Green
    else PORTF = LpCObj.getState(REXCommand - 2); // Red
    
    // Start Timer for Result
    TCCR1B = 0x00;
    OCR1A = 0x0D09; // The green or red light will only be flashed for a split second
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    
    // July 9th, 2013 This addition sort of fixed the problem, but sometimes the green won't deliver the reward
    digitalWrite(REXstart, LOW);
    
     
      
    while(isResult);
    

    PORTF = 0x00; // I'm getting the feeling that PORTF is the LED light. 
    digitalWrite(REXoutput, LOW);
    
    // Stop Timer after Result
    TCCR1B = 0x00;
    
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
  
  //stopTimer after Result
  TCCR1B = 0x00;
}

boolean readTouchInputs(){
  
    //read the touch state from the MPR121
//    Wire.requestFrom(0x5A,2); // request 2 bytes from 0x5A
//    
//    byte LSB = Wire.read();
//    byte MSB = Wire.read();
//    
//    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

spval = analogRead(startplate); 
//spval = 100;
//Serial.print("Start plate prox sensor value is ");
//Serial.println(spval);
          
    int i = 0; // Only using one starting panel
      if(spval<300 & (1<<i)){
      
        if(touchStates[i] == 0){
          // Start stopwatch
          if (myStopWatch.isRunning()) {
            myStopWatch.stop();
            myStopWatch.reset();
          }
          myStopWatch.start();
          // Serial.println("Stopwatch started");
          //pin i was just touched
          // Serial.print("pin ");
          // Serial.print(i);
          // Serial.println(" was just touched");
        
        } else if(touchStates[i] == 1){
          // pin i is still being touched
          // if (!myStopWatch.isRunning())
            // myStopWatch.start();
          // If pin is held longer than 500 milliseconds
          if (myStopWatch.elapsed() > 1000) {
            // don't do anything
          } else if (myStopWatch.elapsed() > 100) {
            digitalWrite(REXstart, HIGH);
            // myStopWatch.stop();
            // myStopWatch.reset();
            return false;
          }
        }  
      
        touchStates[i] = 1;      
      } else {
        if(touchStates[i] == 1){
          
          //pin i is no longer being touched
          // Serial.print("pin ");
          // Serial.print(i);
          // Serial.println(" is no longer being touched");
          
          // End the timer
          myStopWatch.stop();
          myStopWatch.reset();
        
          // Hold on! Wait .... 
       }
        
        touchStates[i] = 0;
      }
    digitalWrite(REXstart, LOW);
    return true;
}

//void mpr121_setup(void){
//
//  set_register(0x5A, ELE_CFG, 0x00); 
//  
//  // Section A - Controls filtering when data is > baseline.
//  set_register(0x5A, MHD_R, 0x01);
//  set_register(0x5A, NHD_R, 0x01);
//  set_register(0x5A, NCL_R, 0x00);
//  set_register(0x5A, FDL_R, 0x00);
//
//  // Section B - Controls filtering when data is < baseline.
//  set_register(0x5A, MHD_F, 0x01);
//  set_register(0x5A, NHD_F, 0x01);
//  set_register(0x5A, NCL_F, 0xFF);
//  set_register(0x5A, FDL_F, 0x02);
//  
//  // Section C - Sets touch and release thresholds for each electrode
//  set_register(0x5A, ELE0_T, TOU_THRESH);
//  set_register(0x5A, ELE0_R, REL_THRESH);
// 
//  set_register(0x5A, ELE1_T, TOU_THRESH);
//  set_register(0x5A, ELE1_R, REL_THRESH);
//  
//  set_register(0x5A, ELE2_T, TOU_THRESH);
//  set_register(0x5A, ELE2_R, REL_THRESH);
//  
//  set_register(0x5A, ELE3_T, TOU_THRESH);
//  set_register(0x5A, ELE3_R, REL_THRESH);
//  
//  set_register(0x5A, ELE4_T, TOU_THRESH);
//  set_register(0x5A, ELE4_R, REL_THRESH);
//  
//  set_register(0x5A, ELE5_T, TOU_THRESH);
//  set_register(0x5A, ELE5_R, REL_THRESH);
//  
//  set_register(0x5A, ELE6_T, TOU_THRESH);
//  set_register(0x5A, ELE6_R, REL_THRESH);
//  
//  set_register(0x5A, ELE7_T, TOU_THRESH);
//  set_register(0x5A, ELE7_R, REL_THRESH);
//  
//  set_register(0x5A, ELE8_T, TOU_THRESH);
//  set_register(0x5A, ELE8_R, REL_THRESH);
//  
//  set_register(0x5A, ELE9_T, TOU_THRESH);
//  set_register(0x5A, ELE9_R, REL_THRESH);
//  
//  set_register(0x5A, ELE10_T, TOU_THRESH);
//  set_register(0x5A, ELE10_R, REL_THRESH);
//  
//  set_register(0x5A, ELE11_T, TOU_THRESH);
//  set_register(0x5A, ELE11_R, REL_THRESH);
//  
//  // Section D
//  // Set the Filter Configuration
//  // Set ESI2
//  set_register(0x5A, FIL_CFG, 0x04);
//  
//  // Section E
//  // Electrode Configuration
//  // Set ELE_CFG to 0x00 to return to standby mode
//  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
//  
//  
//  // Section F
//  // Enable Auto Config and auto Reconfig
//  /*set_register(0x5A, ATO_CFG0, 0x0B);
//  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
//  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
//  
//  set_register(0x5A, ELE_CFG, 0x0C);
//  
//}

//
//boolean checkInterrupt(void){
//  return digitalRead(irqpin);
//}


//void set_register(int address, unsigned char r, unsigned char v){
//    Wire.beginTransmission(address);
//    Wire.write(r);
//    Wire.write(v);
//    Wire.endTransmission();
//}

