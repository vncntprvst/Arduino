#define chB  1      // Pin 3 is Interrupt 1 on UNO
#define chA  0     // Pin 2 is Interrpt 0 on UNO 
#define pi 3.14
volatile long clicks; // Variable used to store wheel movements. Volatile is important
                      // for variables that can change in an interrupt
void setup() {
   Serial.begin(9600);
   attachInterrupt(chA, quadA, CHANGE); // CHANGE = whenever input goes HIGH or LOW on chA, call function quadA
   attachInterrupt(chB, quadB, CHANGE); // And for chB
}

void loop(){
//  print distance
  Serial.println(clicks); //2*pi*4/clicks
}
 
void quadA() {
  // If pin 2 (int 0) state CHANGES, this interrupt is called.
  if (digitalRead(chA)==HIGH) { //If this pin (DIO2) is HIGH
    //Check state of DI03:
    if (digitalRead(chB) == HIGH) { //if DI03 is high 
      clicks++; // Wheel is turning backwards/clockwise (mouse running forwards)
    }
    else {
      clicks--; // Wheel is turning forwards / counter-clockwise (mouse "running" backwards):
    }
  }
  else { //IF DI02 is LOW
    //Check state of DI03:
    if (digitalRead(chB) == HIGH) { //if DIO3 is high
      clicks--; // Wheel is turning forwards
    }
    else {
      clicks++; // Wheel is turning backwards
    }
  }
}

void quadB() {
  // If pin 2 (int 0) state CHANGES, this interrupt is called.
  if (digitalRead(chB)==HIGH) { //If this pin (DIO3) is HIGH
    //Check state of DI02:
    if (digitalRead(chA) == HIGH) { //if DI02 is high 
      clicks--; // Wheel is turning forwards / counter-clockwise (mouse "running" backwards):
    }
    else {
      clicks++; // Wheel is turning backwards/clockwise (mouse running forwards)
    }
  }
  else { //IF DI03 is LOW
    //Check state of DI02:
    if (digitalRead(chA) == HIGH) { //if DIO2 is high
      clicks++; // Wheel is turning backwards
    }
    else {
      clicks--; // Wheel is turning forwards
    }
  }
}
