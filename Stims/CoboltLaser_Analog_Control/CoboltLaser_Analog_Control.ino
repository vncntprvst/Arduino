/*
!!! Warning: Analog input to Cobolt controller should not exceed 1V !!!
Use PWM filter (aka  "true analog" output)

Keyboard Commands:
s - start 
p - pause

set serial monitor Line Ending to Newline first

To do: 
add live intensity modulation 

 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int TTLPin = 12;           // Pulse pin
const int LaserTriggerPin=11;
boolean StimStatus = false;      // Stim is on or off
int letterCount;

// stim parameters
int pulseInt = 100; // in % - max set to 1V
int pulseDur = 100; // 1 or 5 ms
int pulseFreq = 2; // low freq: 1, 5Hz ; high freq: 10, 20, 50
//pulseTrain ...

  // convert parameters
int pulseIntVal= round(pulseInt*51/100); // theorethical limit is 51: 255 (max) / 5 (max V), 
                                         // because capa overshoots
                                         // reduce down to 43 for 1 uF capa
                                         // reduce down to 25 for 0.22 uF capa
int pulseDelay = round(round(1000/pulseFreq) - pulseDur);
  
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(TTLPin, OUTPUT);
  pinMode(LaserTriggerPin, OUTPUT);
  
  if (pulseDelay<1) {
    pulseDelay=1;
  }
  Serial.print("Pulse intensity conversion ");
  Serial.print(pulseInt);
  Serial.print("% => pin value of ");
  Serial.println(pulseIntVal);
  
  Serial.print("Pulse frequency conversion ");
  Serial.print(pulseFreq);
  Serial.print("Hz => delay of ");
  Serial.print(pulseDelay);
  Serial.println(" ms");
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
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
  
  if (StimStatus==true) {
//    Serial.println("Pulse");
    digitalWrite(TTLPin, HIGH);   // turn Sync TTL on
    analogWrite(LaserTriggerPin, pulseIntVal);   // trigger laser
    delay(pulseDur);              // wait for a second
    digitalWrite(TTLPin, LOW);    // turn Sync TTL off
    digitalWrite(LaserTriggerPin, LOW);    // end laser pulse
    delay(pulseDelay);
  }
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


