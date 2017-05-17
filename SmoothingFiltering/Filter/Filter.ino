
#include <Filters.h>

// switch button
#define buttonLWpin 11
#define buttonUWpin  12
#define sessionStart 19
unsigned long statusChange = millis();
byte switchPushCounter[2] = {0, 0};
byte switchState[2] = {0, 0};
byte lastswitchState[2] = {0, 0};

// piezo variables
#define piezoPin A0
int baseline = 5;
int piezoVal = 2;           // store read value
int piezoADC;

// filters out changes faster that 10 Hz.
float filterFrequency = 5.0;

// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );   

// serial com variables
int incomingByte = 0;
byte sessionStatus[2] = {0, 0}; // 1/ Run [ON/OFF (1/0)] 2/ Reset (1)

void setup() 
{
  Serial.begin(57600);
  AFMS.begin();  // initiate motor shield with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz
}

void loop() 
{
//  GetDataFromPC(); // keaboard commands (incompatible with Serial plotter)
 ReadButtons(); // switch button commands
  while (sessionStatus[0] == 0){
//   GetDataFromPC();
    ReadButtons();
  }
  while (sessionStatus[0] == 1){ 
    // Read Piezo ADC value in, and convert it to a voltage
//    piezoADC = analogRead(piezoPin);
//      piezoADC = cumSumAnalog(); //peakAnalog();

  lowpassFilter.input( analogRead( piezoPin ) );
  // do something else

      Serial.println(lowpassFilter.output());
//    float piezoV = piezoADC / 1023.0 * 5.0;
//    Serial.println(piezoV); // Print the voltage.
  
//    if (piezoADC>baseline) {
////      Serial.println("touch detected");
//        Reward(rewardSolenoid,40);
//        delay(50);
//    }
  }
}

void ReadButtons() {

 if  (digitalRead(sessionStart) == HIGH) {
   sessionStatus[0]=1; 
//   Serial.println("session is ON!");
 } else { 
   sessionStatus[0]=1;
//   Serial.println("session is OFF!");
 }
  
//  if (millis() - statusChange > 500){
//    switchState[0] = digitalRead(buttonUWpin);
//    switchState[1] = digitalRead(buttonLWpin);
////    Serial.print("Right push ");
////    Serial.print(switchState[0]);
////    Serial.print(" ; Left push ");
////    Serial.println(switchState[1]);
////    
//    if (switchState[0] != lastswitchState[0]) {
//      if (switchState[0] == HIGH) {
//  //      switchPushCounter[0]++;
//          if (sessionStatus[0] == 1) { 
//            Reward(rewardSolenoid,40);
////            Serial.println("reward");
//          } else {
//            RewardFlush();
//          }
//  //      Serial.println("on");
//  //      Serial.print("number of up button pushes:  ");
//  //      Serial.println(switchPushCounter[0]);
//      } else {
//  //      Serial.println("off");
//      }
//      statusChange = millis();
//      lastswitchState[0] = switchState[0];
//    }
//    if (switchState[1] != lastswitchState[1]) {
//      if (switchState[1] == HIGH) {
//  //      switchPushCounter[1]++;
//          sessionStatus[0] = !sessionStatus[0];
//          if (sessionStatus[0] == 1) { 
////            Serial.println("Session on");
//          } else {
////            Serial.println("Session off");
//          }
//  //      Serial.print("number of left button pushes:  ");
//  //      Serial.println(switchPushCounter[1]);
//      } else {
//  //      Serial.println("off");
//      }
//  //    delay(50);
//      statusChange = millis();
//      lastswitchState[1] = switchState[1];
//    }
//  }
 }

int cumSumAnalog() {
// see also runningMedian Class for Arduino: http://playground.arduino.cc/Main/RunningMedian
//  float piezoArray[2] = {analogRead(piezoPin)};
//  int piezoVal;
//  int minMax;
  int cumVal = analogRead(piezoPin); // analogRead(piezoPin);
  for (int inc=0; inc < 9; inc++ ){
//      piezoVal=analogRead(piezoPin);
      cumVal= cumVal + analogRead(piezoPin);
//      if (piezoArray[0] > piezoVal) {
//        piezoArray[0]=piezoVal;
//        }
//      peakVal = max(peakVal, piezoVal);
   }
//  minMax=piezoArray[1]-piezoArray[0];
//  Serial.print("peak to peak value is ");
//  Serial.println(peakVal);
  return cumVal; //peakVal;
}




