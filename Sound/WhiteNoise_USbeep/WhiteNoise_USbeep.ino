/*************************************************** 
  Based on code for Adafruit TPA2016 Class D Amplifier Breakout
*/

// produces white noise or single freq tone
// depending on number of 

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
int pitch = map(700, 400, 1000, 120, 1500);

// instruction
unsigned long sound_type=0;
//unsigned long stop_noise=0;
int standby = 1;

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
  while (digitalRead(PotPin) == LOW && standby == 1) {
    //wait for instructions
//    playTone(400,100);
//    delay(1000);
  }
//  playTone(150,100); // control beep to check if exits while loop
                       // need to adjust first TTL duration for that 100ms 
  standby = 0;
  sound_type=ReadInstruction();
  unsigned long initTime = millis();
//  if ((unsigned long)(millis() - initTime) > 2000) {
    if (sound_type==1){
      while ((unsigned long)(millis() - initTime) < 2000){
  // changed from fixed duration to having WN on until panel rotation is done
//        while (digitalRead(PotPin) == LOW) 
       generateNoise(frequency);
//       stop_noise=ReadInstruction();
  //     SetGain();
//       while (stop_noise==0);
      }
//      delay(200); // just waiting for TTL to come down 
      standby =1;
    }
    else if (sound_type > 1){
       // generate beep
  //     for(uint8_t i=300; i>250; i--)
        playTone(300,50); //("tone",duration)
        playTone(200,50);
  //     for(uint8_t i=350; i<400; i++)
  //      playTone(i,9); //("tone",duration)
//      delay(200);
      standby =1;
    }
//  }
  sound_type=0;
}

// Play a tone for a specific duration.  value is not frequency to save some
//   cpu cycles in avoiding a divide.  
void playTone(int16_t tonevalue, int duration) {
  for (long i = 0; i < duration * 1000L; i += tonevalue * 2) {
     digitalWrite(RspeakerPin, HIGH);
     delayMicroseconds(tonevalue);
     digitalWrite(RspeakerPin, LOW);
     delayMicroseconds(tonevalue);
  }     
}

unsigned long ReadInstruction() {

  unsigned long highCounter = 0;
  int pulse = LOW;
  int lastPulse = LOW;
  unsigned long initTime = millis();
  
  initTime = millis();
  
  while ((unsigned long)(millis() - initTime) < 10){
    // check incoming TTL for next 10ms
    pulse = digitalRead(PotPin);
    if (pulse != lastPulse) { // pulse has changed
      lastPulse = pulse;
      if (pulse == HIGH) highCounter++;
    } 
  }
  return highCounter;
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
