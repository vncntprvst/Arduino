/*
Keyboard Commands:
s - start 
p - pause
 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int LED_Pin = 8;           
const int TTL1_Pin=9;            // Pulse pin
boolean StimStatus = false;      // Stim is on or off
int letterCount;

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
  
  if (StimStatus==true) {
//    Serial.println("Pulse");
    digitalWrite(LED_Pin, HIGH);   // turn green LED on
    digitalWrite(TTL1_Pin, HIGH);   // trigger laser
    delay(2);              // wait for a 25ms
    digitalWrite(LED_Pin, LOW);    // turn green LED off
    digitalWrite(TTL1_Pin, LOW);    // end laser pulse
    delay(98); // wait for a 475ms
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


