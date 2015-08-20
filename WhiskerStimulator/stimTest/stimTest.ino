int pin = 13;

void setup() {
  // put your setup code here, to run once:
pinMode(pin,INPUT);
Serial.begin(9600);
}
float freq;
    unsigned int currentT;
    unsigned int lastT;
    unsigned int T;
    int lastState = 0;
    int currentState;
void loop() {
 // freq = (1e6)/pulseIn(pin, HIGH,5e6);
 //freq = pulseIn(pin, HIGH,5e6)/1000;
      currentState = digitalRead(pin);
      if(currentState == 1 && lastState == 0){
          currentT = millis();
          T = currentT - lastT;
          Serial.println(T);
          lastT = currentT;
      }
      lastState = currentState;

}
