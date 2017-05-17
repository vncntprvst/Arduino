
#include <Filters.h>
#include <RunningMedian.h>

// piezo variables
#define piezoPin A0

// filters out changes faster that 10 Hz.
float filterFrequency = 10.0;

// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );   

// Running median over 10 samples
RunningMedian samples = RunningMedian(10);
RunningMedian samples2 = RunningMedian(2);
// 
long medianVal;
long averageVal;
  
void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  lowpassFilter.input( analogRead( piezoPin ) );
  samples.add(lowpassFilter.output());
  
  medianVal = samples.getMedian();
  
//  Serial.print(lowpassFilter.output());  
//  Serial.print("\t");  
//  Serial.println(abs(lowpassFilter.output()-medianVal));
  samples2.add(abs(lowpassFilter.output()-medianVal));
  averageVal = samples2.getAverage();
//    Serial.print("\t"); 
  Serial.println(abs(lowpassFilter.output()-medianVal)-averageVal);
  
//  delay(10);
}


