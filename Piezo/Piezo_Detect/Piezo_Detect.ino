
#include <Filters.h>
#include <Average.h>

// lick sensor variables
#define piezoPin A0
#define lickTTLPin 30
float filterFrequency = 10.0; // filters out changes faster that 10 Hz.
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );  // create a one pole (RC) lowpass filter
Average<long> sampleTouch(10);
byte cumulativeTouch = 0 ;
byte mask  = 7; // 7 => 111 (3 successive detection); 15 => 1111
long centeredVal;
long SNR;
unsigned long touchTime = 0;
#define lickInterval 100 // adjust to constraint max allowed lick frequency
#define rewardDuration 80 // 40 calibrated to 1ul
#define SNRthreshold 4
 
void setup(){
  Serial.begin(115200);
  pinMode(lickTTLPin, OUTPUT);
  digitalWrite(lickTTLPin, LOW);
 }

void loop(){
        if (touchPiezo()){ // listen for licks
//           Serial.println("lick detected");
          sendTTL(lickTTLPin,0);
        }
}

bool touchPiezo(){
  lowpassFilter.input( analogRead( piezoPin ) );
  centeredVal=abs(lowpassFilter.output()-sampleTouch.mean());
  sampleTouch.push(centeredVal);
//   Serial.println(centeredVal);
  SNR = max((sampleTouch.mean()/(max(sampleTouch.stddev(),0.01))),1); // no dividing by 0 and SNR never lower than 1
  // Serial.print("\t");
  // Serial.println(3*SNR);
  cumulativeTouch<<=1;
  if (centeredVal>(SNRthreshold*SNR)){
    // detect.push(1);
    cumulativeTouch = cumulativeTouch | 1;
    //    Serial.println("above SNR");
  } else {
    // detect.push(0);
    cumulativeTouch = cumulativeTouch | 0;
  }
  //  Serial.println(cumulativeTouch & mask);
  if ((cumulativeTouch & mask) == mask && ((millis()-touchTime)>lickInterval)){//((detect.mean()*4)>=2) {
//     Serial.print("Touch detected with mean value ");
//     Serial.println(sampleTouch.mean());
    touchTime = millis(); //keep track of time
    return(true);
  } else {
    return(false);
  }
}

void sendTTL(int TTLpin, int instruct){
  digitalWrite(TTLpin, HIGH);
  delay(25);
  digitalWrite(TTLpin, LOW);
  switch (instruct) {
    case 0:
    break; // that's it
    case 1:
    // 2 TTL pulse
    delay(25);
    digitalWrite(TTLpin, HIGH);
    delay(25);
    digitalWrite(TTLpin, LOW);
    break;
    case 2:
  // one more
    delay(25);
    digitalWrite(TTLpin, HIGH);
    delay(25);
    digitalWrite(TTLpin, LOW);
    delay(25);
    digitalWrite(TTLpin, HIGH);
    delay(25);
    digitalWrite(TTLpin, LOW);
    break;
  }
}


