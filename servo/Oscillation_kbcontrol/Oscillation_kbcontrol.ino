/*
// 4/19/16 two pots: one controls angle, the other the delay
//things go astray around 25 degrees and 2ms delay
// 4/20/16  Added TTL out. 
//          Added else statement to keep parameters fixed during stimulation

// Could add angle categories or smoothing array, to keep constant values
// or maybe keyboard control angle and frequency? Or at least value setting session

// Add vertical control

// Add TTL out. 
 */
 
#include <Servo.h> 
 
int anglePin = 0;  
int delayPin = 1;
int servoPin = 9;
const int startingAngle = 10; // starting position
int setAngle = startingAngle;
int angle = 0;
int setDelay = 1;
Servo stimServo; 

//Sync
const int TTLPin = 11;
const int LEDPin = 12;

// serial com
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean StimStatus = false;      // Stim is on or off
int letterCount;
 
void setup() 
{ 
  Serial.begin(9600); 
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(TTLPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  stimServo.attach(servoPin);  
  stimServo.write(setAngle);
} 
 
void loop(){ 

if (stringComplete) {
  ReadInstructions();
}

if (StimStatus==true) {
//    TTLpulse();
    for(angle = startingAngle; angle < (setAngle+startingAngle); angle++)
    {
      stimServo.write(angle);
      digitalWrite(TTLPin, HIGH);
      digitalWrite(LEDPin, HIGH);
      delay(setDelay);                   
    }
    for(angle = (setAngle+startingAngle); angle > startingAngle; angle--)
    {
      stimServo.write(angle);
      digitalWrite(TTLPin, LOW);
      digitalWrite(LEDPin, LOW); 
      delay(setDelay);     
    }
  } else {
    digitalWrite(TTLPin, LOW);
    digitalWrite(LEDPin, LOW);
    stimServo.write(startingAngle);
    ParameterScaling(analogRead(anglePin),analogRead(delayPin));
    delay(500);
  }
} 

void ParameterScaling(unsigned int angleReading, unsigned int delayReading){
  setAngle = (angleReading / 50)+1;// 1 to 21
  setDelay = (delayReading / 75)+2;// 2 to 15
//  Serial.print("anglePin: ");
//  Serial.print(angleReading);
  Serial.print("Set target angle: ");
  Serial.print(setAngle);
//  Serial.print("delayPin : ");
//  Serial.print(delayReading);
//  Serial.print("; setDelay: ");
//  Serial.print(setDelay);
  Serial.print("; target frequency: ");
  Serial.println(1000/(2*setDelay*setAngle));
}

void TTLpulse() {
  digitalWrite(TTLPin, HIGH);
  digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(TTLPin, LOW);
  digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  
}

void ReadInstructions(){
    // clear the string:
  if (inputString[0]=='s') {
    Serial.println("Start");
    StimStatus = true;
  }
  else if (inputString[0]=='p') { // inputString.equalsIgnoreCase("p")
    Serial.println("Pause");
    StimStatus = false;
  }
  else {
    for (letterCount = 0; letterCount < sizeof(inputString) - 1; letterCount++){
      Serial.print(letterCount, DEC);
      Serial.print(" = ");
      Serial.write(inputString[letterCount]);
      Serial.println();
    }
//      Serial.println(sizeof(inputString)); 
//      Serial.println(StimStatus);
  }
  inputString = "";
  stringComplete = false;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
  }
}
}
