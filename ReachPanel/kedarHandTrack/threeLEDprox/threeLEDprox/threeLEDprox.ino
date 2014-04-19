#include <avr/interrupt.h>
#include <LEDproxCoup.h>
#define nMaxREXCommands 20

LEDproxCoup LpCObj;
byte REXCommands[nMaxREXCommands] = {3,9,6,9,3,6,3,6,3,9,3,0,6,3,6,6,9,3,9};
byte LEDCommand=0x00;
byte sensorState=0xff;

volatile bool isTask, isResult, isActive;

void setup()
{
  LpCObj.Init();
  Serial.begin(9600);
}

void loop()
{
  /*for (int iCommand = 0;iCommand < nMaxREXCommands;iCommand++)  
  {
    isTask = true;
    isActive = false;
    
    LEDCommand = LpCObj.getState(REXCommands[iCommand]);
    //startTimer for Task
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    
    while(isTask)
    {
      PORTF = LEDCommand;
      
      if(~isActive)
      {
        sensorState = PINK & 7;
        isActive = ((sensorState)==0b00000111) ? false : true;
      }  
      Serial.println(isActive);
    }
  }*/
  for (int iCommand = 0;iCommand < nMaxREXCommands;iCommand++)  
  {
    isTask = true;
    isResult = true;
    isActive = false;
    
    LEDCommand = 0x00;
    sensorState = 0xff;
    //getRexCommand
    LEDCommand = LpCObj.getState(16*REXCommands[iCommand]);
    
    //startTimer for Task
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    
    while(isTask)
    {
      PORTF = LEDCommand;
      if(~isActive)
      {
        sensorState = PINK & 7;
        isActive = (sensorState == 7) ? false : true;
      }
    }
    //stopTimer after Task
    //TCCR1B = 0x00;
        
    if(LpCObj.checkProx(LEDCommand,sensorState))
    {
      PORTF = LpCObj.getState(16*(REXCommands[iCommand] - 1));
      Serial.println(PORTF);
    }
    else
      PORTF = LpCObj.getState(16*(REXCommands[iCommand] - 2));
      
    //startTimer for Result
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    while(isResult);
    
    PORTF = 0x00;
    //stopTimer after Result
    //TCCR1B = 0x00;
 
  }
  
}

ISR(TIMER1_COMPA_vect)
{
  if(isTask)  
  {
    isTask = false;  
    isActive = true;
  }
  else
    isResult = false;
  
  TCCR1B = 0x00;
}

/*void inline callTimer(bool startIt)
{
  TCCR1B |= startIt ? ((1 << WGM12) | (1 << CS12) | (1 << CS10)) : (~TCCR1B);
}*/


