/* Program to run the whisker stimulator off the Arduino. Pulses are sent
to a particular magnet selected by a joystick. The pulse frequency can be
altered by rotating a potentiometer. Able to switch between 4-magnet and
single-magnet modes.
*/

// Pins for each magnet
const int m1 =  3;         
const int m2 =  5;            
const int m3 = 6;
const int m4 = 9;
const int m5 = 10;

// Pins for joystick
// Joystick is aligned when m1 is facing up
const int j1 = 2; // up, green, corresponds to m1
const int j2 = 4; // right, blue, corresponds to m2
const int j3 = 7; // down, white, corresponds to m3
const int j4 = 8; // left, yellow, corresponds to m4

// Pin for single magnet/4 magnet select switch
const int selectPin = 12; 

// Pin for TTL out
const int TTLPin1 = 11; // Digital TX p 

// main variables
float currentTime; // Variable to check current time (ms)
float previousTime = 0; // Variable compared against currentTime 
                        // to decide whether to send pulse 
float pulseTimes; // Variable to store time of each pulse start(ms)
float pulseTimee; // Variable to store time of each pulse end (ms)

float startTime;

void setup() {
  Serial.begin(9600);
  // initialize the magnet pins as outputs:
  pinMode(m1, OUTPUT);   
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT); 
  pinMode(m4, OUTPUT);
  pinMode(m5, OUTPUT);
  
  pinMode(A0, INPUT); // Analog input, reads voltage from potentiometer
  
  // initialize the joystick pins as inputs:
  pinMode(j1, INPUT);
  pinMode(j2, INPUT);
  pinMode(j3, INPUT);
  pinMode(j4, INPUT);
  
  pinMode(selectPin, INPUT);
  pinMode(TTLPin1, OUTPUT);
  
}

int mag; // variable to store current magnet to pulse
int lastmag; // variable to store the previous magnet that was pulsed

void loop(){

int selectMode = digitalRead(selectPin);
if(selectMode == HIGH){
lastmag = mag;
mag = readJoystick(); // Determine which magnet to pulse from joystick
} else{
 mag = m5; 
}
//Serial.print("Magnet "); Serial.print(mag); Serial.print(" selected");

int Aval = analogRead(A0); // Get voltage across potentiometer

// returns value between 0 to 1023
float interval = 1000000; //50+Aval-(0.2*Aval); // Scale value to desired frequency range (~1 to 20Hz)
//Serial.print("interval "); Serial.println(interval);

currentTime = micros();
//Serial.print("currentTime "); Serial.println(currentTime);
//Serial.print("time diff "); Serial.println(currentTime - previousTime);
if(currentTime - previousTime >= interval-650){
  // If enough time has elapsed, send pulse to mag
//    pulseTimes = millis();
//    Serial.print("pulseTime "); Serial.println(pulseTimes);
    pulse(mag);
  //  pulseTimee = millis();
    //Serial.print("pulseTime "); Serial.println(pulseTimee);
    
    previousTime = currentTime;
  //  Serial.print("previousTime "); Serial.println(previousTime);
  //  delay(30);
}

//float freq = 1000/interval; // pulse frequency in Hz
//Serial.print(" at frequency "); Serial.print(freq); Serial.println(" Hz"); // Display frequency
}

// Reads the direction of the joy stick and sets the corresponding 
// magnet as the one to pulse. If nothing is selected, return
// the previously selected magnet
int readJoystick(){
  int j1State = digitalRead(j1);
  int j2State = digitalRead(j2);
  int j3State = digitalRead(j3);
  int j4State = digitalRead(j4);
  
  if(j1State==LOW){
    return m1;
  }
  else if(j2State==LOW){
    return m2;
  }
  else if(j3State==LOW){
    return m3;
  }
  else if(j4State==LOW){
    return m4;
  }
  else{
    // If nothing has been selected, keep pulsing the same magnet
    return lastmag;
  }
}


// Sends a pulse to a magnet
// pulse width = 30 ms
// rise/fall time = 10 ms => max 20Hz
void pulse(int m){
  TTLpulse(TTLPin1); // send TTL pulse at begining each stim  
  for(int i = 0; i<250; i+=25){
    analogWrite(m, i); 
    delay(1);
  } 
  analogWrite(m,255);
  delay(30);
  for(int i = 255; i>0; i-=25){
    analogWrite(m, i); 
    delay(1);
  }
}

//Send TTL pulse
void TTLpulse(int ttlpin){
  digitalWrite(ttlpin, HIGH); 
  delay(50);
  digitalWrite(ttlpin, LOW); 
}
