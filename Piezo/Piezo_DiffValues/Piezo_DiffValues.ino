
#define sampleWindowSize 600
#define vibThd 20

unsigned short int baseline = 1;
unsigned short int piezoVal;

short int readIndex = 0;
short int touchDetect = 0;
unsigned short int maxDiff;
const int piezoPin = A0; // Piezo output

void setup() 
{
  Serial.begin(9600);
  findmaxdiff();
//  baseline=maxDiff;
//  Serial.print("baseline ");
//  Serial.println(baseline);
}

void loop() 
{
//  if readIndex>4
  maxDiff = findmaxdiff();
//  if (maxDiff>baseline){
//    maxDiff=maxDiff-baseline;
//  } else {
//    maxDiff=0;
//  }
  Serial.println(maxDiff);
  if (maxDiff>vibThd) {
//    touchDetectArray[readIndex]=1;
//    Serial.println("touch detected");
  }
  readIndex++;
}

int findmaxdiff(){
  unsigned short int maxVal=0;
  unsigned short int minVal=1024;
  
  for(int i = 0; i < sampleWindowSize ; ++i){
    piezoVal = analogRead(piezoPin);
    minVal = min(minVal, piezoVal);
    maxVal = max(maxVal, piezoVal);
  }
  return abs(maxVal-minVal);
}



