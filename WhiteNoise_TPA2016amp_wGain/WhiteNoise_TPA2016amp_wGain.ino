/*************************************************** 
  Based on code for Adafruit TPA2016 Class D Amplifier Breakout
*/

//#include <Wire.h>
//#include "Adafruit_TPA2016.h"

#include <TinyWireM.h>
#include "Adafruit_TPA2016_tiny.h"

//  pin setup
const int RspeakerPin = 3;
//const int LspeakerPin = 4;
const int PotPin = 1; 
//const int PotAddress = 2; // on trinket Pin 4 is A2

Adafruit_TPA2016 audioamp = Adafruit_TPA2016();

//  for noise generation
unsigned long int reg;
byte gainValue = 0; 
int amp_gainValue= 0;
int fqValue = 10;
int frequency = map(fqValue, 0, 1023, 0, 1000);

void setup() {
  
  pinMode(RspeakerPin,OUTPUT);
//  pinMode(LspeakerPin,OUTPUT);
  pinMode(PotPin,INPUT);
  
  // Arbitrary inital value; must not be zero
  reg = 0x55aa55aaL; //The seed for the bitstream. It can be anything except 0.
    
// Serial.begin(9600);
// Serial.println("TPA2016 Audio Test"); 
  audioamp.begin(); 
  audioamp.enableChannel(true, false);
 
//   // Turn off AGC for the gain test
//  audioamp.setAGCCompression(TPA2016_AGC_OFF);
//  // we also have to turn off the release to really turn off AGC
//  audioamp.setReleaseControl(0);

  audioamp.setGain(-28);
}

void loop() {
  if (digitalRead(PotPin) == HIGH){
     generateNoise(frequency);
//     SetGain();
  }
}

void generateNoise(int frequency) {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
   
  // Extract four chosen bits from the 32-bit register
  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;

  // EXOR the four bits together
  lobit = b31 ^ b29 ^ b25 ^ b24;
  
  // Shift and incorporate new bit at bit position 0
  newr = (reg << 1) | lobit;
  
  // Replace register with new value
  reg = newr;
  
  // Drive speaker pin from bit 0 of 'reg'
  digitalWrite(RspeakerPin, reg & 1);
  
  // Delay (50) corresponds to 20kHz, but the actual frequency of updates
  // will be lower, due to computation time and loop overhead
  delayMicroseconds(frequency);    // Changing this value changes the frequency.
}

//void SetGain() {
//  // update the gain, from -28dB up to 30dB
//    gainValue = analogRead(PotAddress);
//    amp_gainValue=ScaleGain(gainValue);
//    audioamp.setGain(amp_gainValue);
//}
//
//int ScaleGain(int gainval) {
//  int scaledVal; 
//  if (gainval>9){
//     scaledVal=((gainval/9)*2)-28;
//  }else{
//     scaledVal=-28;
//  }
//  return scaledVal;
//}
   
