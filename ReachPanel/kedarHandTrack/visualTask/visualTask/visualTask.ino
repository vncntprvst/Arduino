#include <avr/interrupt.h>
#include <LEDproxCoup.h>

#define nMaxREX 10

LEDproxCoup lpc;
byte REXCommand, LEDCommand, sensorState;
byte REX[nMaxREX] = {144,48,96,144,96,48,144,96,48,96};
volatile bool isTask, isResult, isActive, isTimer;

void setup()
{
  lpc.Init();
  Serial.begin(9600);
}

void loop()
{
  Serial.println("NOT in TASK");
  if(isTask)
  {
    Serial.println("IN TASK");
    for (int i = 0;i < nMaxREX;i++)
    {
      //Read from Breakout Board and light respective LED
      //REXCommand = PIND & 0xF0;
      REXCommand = REX[i];
      LEDCommand = lpc.getState(REXCommand);
      
      //Enable INT1 Interrupt and Timer1
      //EIMSK |= (1 << INT1);
      TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
      
      //Wait for INT1 or Timer1 to fire
      while(~isActive & isTimer)
      {
        PORTF = LEDCommand;
        sensorState = PINK & 0x07;
        
        isActive = (sensorState == 0x07) ? false : true;
      }
      
      //Give Result based on whether we got the correct State
      if(lpc.checkProx(LEDCommand,sensorState))
        PORTF = lpc.getState(REXCommand - 16);//green
      else
        PORTF = lpc.getState(REXCommand - 2*16);//red
      
      //Enable Timer1 to time result LED and wait till it fires
      TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
      while(isResult);
     }
    //Enable INT0 for next REXCommand
    EIMSK |= (1 << INT0);  
      
  }
}

//External Interrupt 0 Handler
ISR(INT0_vect)
{
  isTask = true;
  isResult = true;
  isActive = false;
  isTimer = true;
  
  EIMSK &= ~(1 << INT0);
}

//External Interrupt 1 Handler
ISR(INT1_vect)
{
  isActive = true;
  
  EIMSK &= ~(1 << INT1);
}

//Timer1 Output-Compare Interrupt Handler
ISR(TIMER1_COMPA_vect)
{
  if(isTask)  
  {
    isTask = false;  
    isActive = true;
    isTimer = false;
  }
  else
    isResult = false;
  
  TCCR1B = 0x00;
}
