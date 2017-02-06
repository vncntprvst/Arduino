
// Stepper 1 controlled through ST-M5045

// Stepper 1
#define stepPinS1 8
#define directionPinS1 9
#define enablePinS1  12
#define buttonCWpin 10
#define buttonCCWpin  11

// other defs
boolean buttonCWpressed = false;
boolean buttonCCWpressed = false;
boolean railMotion = false;

int NumForwardSteps=800;
int NumReverseSteps=800;
int Speed = 300;  //control how fast to "step" the motor
int NumSteps = 200;  //how many steps does the stepper take to make a rotation  (the actual motor, not factoring in "micro" steps)
int MicroSteps = 2; //how many microsteps is the driver set to
int FullRotation = NumSteps * MicroSteps;


void setup()
{  
//  Serial.begin(9600);

  pinMode(directionPinS1, OUTPUT);
  pinMode(stepPinS1, OUTPUT);
  pinMode(enablePinS1, OUTPUT);
  pinMode(buttonCWpin, INPUT);
  pinMode(buttonCCWpin, INPUT);

  digitalWrite(directionPinS1, LOW);
  digitalWrite(stepPinS1, LOW);
}

void loop() { 
 readButtons();
  if (buttonCCWpressed || buttonCWpressed == true) {
    digitalWrite(enablePinS1, HIGH);
    actOnButtons();
  } else {
//  Serial.println("idle");
  }
}

void readButtons() {
 buttonCCWpressed = false;
 buttonCWpressed = false;
 
 if (digitalRead(buttonCWpin) == HIGH) {
 buttonCWpressed = true;
// Serial.println("CW rotation");
 }
 if (digitalRead(buttonCCWpin) == HIGH) {
 buttonCCWpressed = true;
// Serial.println("CCW rotation");
 }
}

void actOnButtons() {
 if (buttonCWpressed == true) {
    digitalWrite(directionPinS1,LOW);//Go Forward
    for(int i = 0; i<NumForwardSteps;i++){//Step NumForwardSteps forward with each step taking 2*Speed microseconds.
        digitalWrite(stepPinS1,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(stepPinS1,LOW);
        delayMicroseconds(Speed);
    }
 }
 if (buttonCCWpressed == true) {
    digitalWrite(directionPinS1,HIGH);//Reverse
    for(int i = 0; i<NumReverseSteps;i++){//Step NumReverseSteps backwards
        digitalWrite(stepPinS1,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(stepPinS1,LOW);
        delayMicroseconds(Speed);
    }
 }
 buttonCCWpressed = false;
 buttonCWpressed = false;
}





