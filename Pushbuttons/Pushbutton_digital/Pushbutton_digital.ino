/*
  State change detection (edge detection)

 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.

/* modified from State Change Detection */

// this constant won't change:
const int  red_buttonPin = 15; 
//const int  green_buttonPin = 19;    
//const int ledPin = 13;       

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
//int green_buttonState = 0;
int red_buttonState = 0;
int lastButtonState = 0;     // previous state of the button

void setup() {

  pinMode(red_buttonPin, INPUT);
//  pinMode(green_buttonPin, INPUT);

  Serial.begin(115200);
}


void loop() {
  // read the pushbutton input pin:
//  green_buttonState = digitalRead(green_buttonPin);
  red_buttonState = digitalRead(red_buttonPin);
  Serial.println(red_buttonState);
  // compare the buttonState to its previous state
  if ((red_buttonState != lastButtonState)){
    // if the state has changed, increment the counter
    if ((red_buttonState == HIGH)){
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }

  lastButtonState = red_buttonState;


  // turns on the LED every four button pushes by
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
//  if (buttonPushCounter % 4 == 0) {
//    digitalWrite(ledPin, HIGH);
//  } else {
//    digitalWrite(ledPin, LOW);
//  }

}









