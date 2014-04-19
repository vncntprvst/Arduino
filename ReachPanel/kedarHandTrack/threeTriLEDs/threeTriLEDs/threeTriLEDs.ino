#include <triColorLED.h>

triColorLED tCLObj;
//byte REXCommands[20] = {4,2,0,9,0,1,7,6,3,0,1,2,3,4,5,6,7,8,9};
byte REXCommand;
byte LEDCommand;

void setup()
{
  DDRB = 0xff;
  DDRC = 0b00001111;
  Serial.begin(9600);
  tCLObj.generateGrid();
}

void loop()
{
  /*
  for (int iCommand = 0;iCommand < 20;iCommand++)  
  {
    LEDCommand = tCLObj.getState(REXCommands[iCommand]);
    PORTB = LEDCommand;
    Serial.print("Rex Command = ");
    Serial.print(REXCommands[iCommand]);
    Serial.print("|LED Command = ");
    Serial.print(LEDCommand);
    Serial.print("|PORTB = ");
    Serial.println(PORTB);
    delay(1000);

  }
  */
  REXCommand = PINC;
  LEDCommand = tCLObj.getState(REXCommand);
  PORTB = LEDCommand;
  Serial.print("PINC = ");
  Serial.print(PINC);
  Serial.print("|PORTB = ");
  Serial.println(LEDCommand);
  
}


