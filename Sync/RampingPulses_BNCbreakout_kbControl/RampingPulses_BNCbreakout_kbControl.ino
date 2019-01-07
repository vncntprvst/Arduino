/*
Keyboard Commands:
s - start
p - pause
 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int LED_Pin = 10;
const int TTL1_Pin=9;            // Pulse pin
boolean StimStatus = false;      // Stim is on or off
int letterCount;

const unsigned long pulsesPerTrain = 4;          // Number of pulses in pulse train (ms)
unsigned long pulseFrequency = 2;          // frequency in Hz (ms)
unsigned long pulseDuration = 50;           // pulse duration (ms)
unsigned long interPulseInterval = int(1000/pulseFrequency)-pulseDuration;          // ISI (ms)
unsigned long trainInterval = 2000 + (pulsesPerTrain*pulseDuration) + ((pulsesPerTrain-1)*interPulseInterval);         //delay between pulse sets (ms)
unsigned long previousTime = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(LED_Pin, OUTPUT);
  pinMode(TTL1_Pin, OUTPUT);
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
    digitalWrite(LED_Pin, HIGH);   // turn Sync TTL on
    digitalWrite(TTL1_Pin, HIGH);   // trigger laser
    delay(pulseDuration);              // wait for a second
    digitalWrite(LED_Pin, LOW);    // turn Sync TTL off
    digitalWrite(TTL1_Pin, LOW);    // end laser pulse
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
