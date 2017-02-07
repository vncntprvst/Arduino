
// switch button
#define buttonRWpin 11
#define buttonLWpin 12
unsigned long statusChange = millis();
byte switchPushCounter[2] = {0, 0};
byte switchState[2] = {0, 0};
byte lastswitchState[2] = {0, 0};
const int ledPin =  13;

void setup() 
{
  Serial.begin(115200);
  pinMode(buttonRWpin, INPUT);
  pinMode(buttonLWpin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buttonRWpin, LOW);
  digitalWrite(buttonLWpin, LOW);
  digitalWrite(ledPin, LOW);
}

void loop() 
{
 readButtons(); // switch button commands
}

void readButtons() {
  switchState[0] = digitalRead(buttonRWpin);
  switchState[1] = digitalRead(buttonLWpin);
  Serial.print("Right push ");
  Serial.print(switchState[0]);
  Serial.print(" ; Left push ");
  Serial.println(switchState[1]);
  if (switchState[0] == 1) {
    digitalWrite(ledPin, HIGH);
  }
  
  if ((switchState[0] != lastswitchState[0]) || (switchState[1] != lastswitchState[1])) {
    if (switchState[0] == HIGH) {
      switchPushCounter[0]++;
      Serial.println("on");
      Serial.print("number of up button pushes:  ");
      Serial.println(switchPushCounter[0]);
    } else {
      Serial.println("off");
    }
    if (switchState[1] == HIGH) {
      switchPushCounter[1]++;
      Serial.println("on");
      Serial.print("number of left button pushes:  ");
      Serial.println(switchPushCounter[1]);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastswitchState[0] = switchState[0];
  lastswitchState[1] = switchState[1];
}


