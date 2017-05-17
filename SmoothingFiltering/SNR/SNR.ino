
#include <Filters.h>
//#include <RunningMedian.h>
#include <Average.h>

// piezo variables
#define piezoPin A0

// filters out changes faster that 10 Hz.
float filterFrequency = 10.0;

// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );   

// Running median over 10 samples
//RunningMedian samples = RunningMedian(10);
//RunningMedian samples2 = RunningMedian(2);

Average<long> samples(10);
Average<byte> detect(4);
byte cumulativeTouch = 0 ;
byte mask  = 15;
long centeredVal;
long diffVal;
long SNR;
unsigned long touchTime = 0;

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  lowpassFilter.input( analogRead( piezoPin ) );
  centeredVal=abs(lowpassFilter.output()-samples.mean());
  samples.push(centeredVal);
  Serial.print(centeredVal);
//  lowpassFilter.input( analogRead( piezoPin ) );
//  diffVal=abs(abs(lowpassFilter.output()-samples.mean())-centeredVal);
//  Serial.println(diffVal);
  SNR = samples.mean()/(max(samples.stddev(),0.10));
//  Serial.print("\t");
//  Serial.print(samples.mean());
//  Serial.print("\t");
//  Serial.print(samples.stddev());
  Serial.print("\t");
  Serial.println(5*SNR);
  cumulativeTouch<<=1;
  if (centeredVal>(5*SNR)){
    detect.push(1);
    cumulativeTouch = cumulativeTouch | 1;
//    Serial.println("above SNR");
  } else { 
    detect.push(0); 
    cumulativeTouch = cumulativeTouch | 0;
  }
//  Serial.println(cumulativeTouch & mask);
  if ((cumulativeTouch & mask) == mask && ((millis()-touchTime)>200)){//((detect.mean()*4)>=2) {
//  Serial.println(detect.mean());
    touchTime = millis(); //keep track of time
//    Serial.println(1);
  } else {
//    Serial.println(0);
  }
//  delay(10);
}



