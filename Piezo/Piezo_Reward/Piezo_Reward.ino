/******************************************************************************
When touch detected, give liquid reward
******************************************************************************/

#include <Wire.h> //The shield uses the SDA and SCL i2c pins to control DC and stepper motors.  
                  //On the Arduino UNO these are also known as A4 and A5
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. 
// Solenoid on M2
Adafruit_DCMotor *rewardSolenoid = AFMS.getMotor(2);

// switch button
#define buttonLWpin 11
#define buttonUWpin  12
unsigned long statusChange = millis();
byte switchPushCounter[2] = {0, 0};
byte switchState[2] = {0, 0};
byte lastswitchState[2] = {0, 0};

// piezo variables
#define piezoPin A0
int baseline = 5;
int piezoVal = 2;           // store read value
int piezoADC;

// serial com variables
int incomingByte = 0;
byte SessionStatus[2] = {0, 0}; // 1/ Run [ON/OFF (1/0)] 2/ Reset (1)

void setup() 
{
  Serial.begin(115200);
  AFMS.begin();  // initiate motor shield with the default frequency 1.6KHz
  //AFMS.begin(1000);  // to apply a different frequency, here 1KHz

// turn off current in solenoid coil
  rewardSolenoid->run(RELEASE);
}

void loop() 
{
//  GetDataFromPC(); // keaboard commands (incompatible with Serial plotter)
 ReadButtons(); // switch button commands
  while (SessionStatus[0] == 0){
//   GetDataFromPC();
    ReadButtons();
  }
  if (SessionStatus[0] == 1){ 
    // Read Piezo ADC value in, and convert it to a voltage
//    piezoADC = analogRead(piezoPin);
      piezoADC = cumSumAnalog(); //peakAnalog();
      Serial.println(piezoADC);
//    float piezoV = piezoADC / 1023.0 * 5.0;
//    Serial.println(piezoV); // Print the voltage.
  
    if (piezoADC>baseline) {
//      Serial.println("touch detected");
        Reward(rewardSolenoid,40);
        delay(500);
    }
  }
}

void GetDataFromPC() {
  if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
//                Serial.print("I received: ");
//                Serial.println(incomingByte, DEC);

                if (incomingByte == 101) { // "e"
//                   Serial.println("stop");
                   SessionStatus[0] = 0;
                } else if (incomingByte == 115){ // "s"
//                   Serial.println("start");
                   SessionStatus[0] = 1;
                } else if (incomingByte == 114){ // "r"
//                   Serial.println("reward");
                   Reward(rewardSolenoid,40);
                }
  }
}

void ReadButtons() {
  if (millis() - statusChange > 500){
    switchState[0] = digitalRead(buttonUWpin);
    switchState[1] = digitalRead(buttonLWpin);
//    Serial.print("Right push ");
//    Serial.print(switchState[0]);
//    Serial.print(" ; Left push ");
//    Serial.println(switchState[1]);
    
    if (switchState[0] != lastswitchState[0]) {
      if (switchState[0] == HIGH) {
  //      switchPushCounter[0]++;
          if (SessionStatus[0] == 1) { 
            Reward(rewardSolenoid,40);
//            Serial.println("reward");
          } else {
            RewardFlush();
          }
  //      Serial.println("on");
  //      Serial.print("number of up button pushes:  ");
  //      Serial.println(switchPushCounter[0]);
      } else {
  //      Serial.println("off");
      }
      statusChange = millis();
      lastswitchState[0] = switchState[0];
    }
    if (switchState[1] != lastswitchState[1]) {
      if (switchState[1] == HIGH) {
  //      switchPushCounter[1]++;
          SessionStatus[0] = !SessionStatus[0];
          if (SessionStatus[0] == 1) { 
//            Serial.println("Session on");
          } else {
//            Serial.println("Session off");
          }
  //      Serial.print("number of left button pushes:  ");
  //      Serial.println(switchPushCounter[1]);
      } else {
  //      Serial.println("off");
      }
  //    delay(50);
      statusChange = millis();
      lastswitchState[1] = switchState[1];
    }
  }
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

void Reward(Adafruit_DCMotor* solenoid,int dur){
  solenoid->setSpeed(255);  
  solenoid->run(FORWARD);
  delay(dur);
  solenoid->run(RELEASE); // cut power to motor
}

void RewardFlush(){
//  Serial.println("flush");
  // flush left
  rewardSolenoid->run(FORWARD);
  rewardSolenoid->setSpeed(255);
  delay(1500);
  rewardSolenoid->run(RELEASE);
  // flush right
//  RightSolenoid->run(FORWARD);
//  RightSolenoid->setSpeed(255);
//  delay(1500);
//  RightSolenoid->run(RELEASE);
}



