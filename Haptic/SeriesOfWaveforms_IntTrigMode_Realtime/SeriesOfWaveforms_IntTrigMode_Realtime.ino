#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;
uint8_t WFlib[] = {64};
//uint8_t effect = 1;

void setup() {
  Serial.begin(9600);
//  Serial.println("DRV test");
  drv.begin();
  drv.selectLibrary(1);
  // I2C trigger by sending 'go' command 
  drv.setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

}

void loop() {
  for (uint8_t WFlibIdx = 0; WFlibIdx < sizeof(WFlib); WFlibIdx++){
    selectWF(WFlib[WFlibIdx]); // select waveform to play
    timedVibration(4000);
    delay(500);
  }
//  Serial.print("Effect #"); Serial.println(effect);
//  drv.setWaveform(0, effect);  // play effect 
//  drv.setWaveform(1, 0);       // end waveform
//  timedVibration(1000);
//  delay(500);
//  effect++;
//  if (effect > 117) effect = 1;
}

void selectWF(int libIdx){
//  drv.setWaveform(1,106);
  drv.setWaveform(1, libIdx);
  drv.setWaveform(2, libIdx);
  drv.setWaveform(3, libIdx);
  drv.setWaveform(4, libIdx);
//  drv.setWaveform(4, libIdx);
//  drv.setWaveform(5, 98);
}

void timedVibration(unsigned long howLong)
{
  unsigned long startedAt = millis();
  while(millis() - startedAt < howLong)
  {
    drv.go();
  }
}
