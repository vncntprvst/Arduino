/*
Keyboard Commands:
s - start 
p - pause
 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const short TTLPin = 4;           // Pulse pin
const short laserTriggerPin=2;
boolean StimStatus = false;      // Stim is on or off
int letterCount;

const unsigned long pulsesPerTrain = 5;          // Number of pulses in pulse train (ms)             
unsigned long pulseFrequency = 10;          // frequency in Hz (ms)                         
unsigned long pulseDuration = 5;           // pulse duration (ms) 
unsigned long interPulseInterval = int(1000/pulseFrequency)-pulseDuration;          // ISI (ms)                           
unsigned long trainInterval = 500 + (pulsesPerTrain*pulseDuration) + ((pulsesPerTrain-1)*interPulseInterval);         //delay between pulse sets (ms)  
unsigned long previousTime = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(TTLPin, OUTPUT);
  pinMode(laserTriggerPin, OUTPUT);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    // clear the string:
    if (inputString[0]=='s') {
      Serial.println("Start");
      StimStatus = true;
//      delay(5000);
//    digitalWrite(TTLPin, HIGH);   // turn Sync TTL on
//    digitalWrite(laserTriggerPin, HIGH);   // trigger laser
//    delay(25);              // wait for a second
//    digitalWrite(TTLPin, LOW);    // turn Sync TTL off
//    digitalWrite(laserTriggerPin, LOW);    // end laser pulse
//    delay(100);
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

  unsigned long currentTime = millis();
  
  if ((StimStatus==true) && (currentTime - previousTime >= trainInterval)) {
    // save the last time you blinked the LED
    previousTime = currentTime;
//    Serial.println("Pulses on");
//  Serial.println(interPulseInterval);
//  Serial.println(trainInterval);
    GenerateTrain();
  }
}

void GenerateTrain() {
 // call SendPulse function pulsesPerTrain times
 for (int i=1; i<=pulsesPerTrain; i++) {  
   SendPulse();
 }
}

void SendPulse() {
    digitalWrite(TTLPin, HIGH);   // turn Sync TTL on
    digitalWrite(laserTriggerPin, HIGH);   // trigger laser
    delay(pulseDuration);              // wait for a second
    digitalWrite(TTLPin, LOW);    // turn Sync TTL off
    digitalWrite(laserTriggerPin, LOW);    // end laser pulse
    delay(interPulseInterval);
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
