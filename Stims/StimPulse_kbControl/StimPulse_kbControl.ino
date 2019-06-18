/*
// Add angle categories or smoothing array, to keep constant values
// or maybe keyboard control angle and frequency? Or at least value setting session

// Add vertical control

// Add TTL out. 
 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int TTLPin = 9;           // Pulse pin
boolean StimStatus = false;      // Stim is on or off
int letterCount;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(TTLPin, OUTPUT);
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
    Serial.println("Pulse");
    digitalWrite(TTLPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(TTLPin, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
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


