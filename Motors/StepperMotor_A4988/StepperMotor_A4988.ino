// testing a stepper motor with a Pololu A4988 driver board
// The onboard led will flash with each step
// this version uses delay() to manage timing

// Pin allocation
const byte directionPin = 2;
const byte stepPin = 3;
//const byte sleepPin = 4;
//const byte resetPin = 5;
//const byte enablePin = 6;
const byte ledPin = 13;

// Motor parameters 
int numberOfSteps = 400;
int pulseWidthMicros = 20;  // microseconds
int millisbetweenSteps = 250; // milliseconds - or try 1000 for slower steps

void setup() { 

  Serial.begin(9600);
  Serial.println("Starting StepperTest");
  digitalWrite(ledPin, LOW);
  
  delay(2000);

//  DDRD = DDRD | B11111100;  // sets pins 2 to 7 as outputs
//                    // without changing the value of pins 0 & 1 (serial com)                 
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
//  pinMode(ledPin, OUTPUT);
//  pinMode(sleepPin, OUTPUT);
//  pinMode(resetPin, OUTPUT);

//  PORTD = PORTD | B00110100;
//  digitalWrite(sleepPin, HIGH);
//  digitalWrite(resetPin, HIGH);
//  digitalWrite(enablePin, LOW);
  
  digitalWrite(directionPin, HIGH);
  for(int n = 0; n < numberOfSteps; n++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros); // this line is probably unnecessary
    digitalWrite(stepPin, LOW);
    
    delay(millisbetweenSteps);
    
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  
  delay(3000);

  digitalWrite(directionPin, LOW);
  for(int n = 0; n < numberOfSteps; n++) {
    digitalWrite(stepPin, HIGH);
    // delayMicroseconds(pulseWidthMicros); // probably not needed
    digitalWrite(stepPin, LOW);
    
    delay(millisbetweenSteps);
    
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}

void loop() { 
}
