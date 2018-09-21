#include "triColorLED.h"

/*
triColorLED::triColorLED()
{
}

triColorLED::~triColorLED()
{
}
*/

byte triColorLED::getState(byte LEDCommand)
{
	switch(LEDCommand)
	{
	case 0:
		return 0;
	case 1:
		return stateGrid[0][0];
	case 2:
		return stateGrid[0][1];
	case 3:
		return stateGrid[0][2];
	case 4:
		return stateGrid[1][0];
	case 5:
		return stateGrid[1][1];
	case 6:	
		return stateGrid[1][2];
	case 7:
		return stateGrid[2][0];
	case 8:
		return stateGrid[2][1];
	case 9:
		return stateGrid[2][2];
	default:
		return -1;
	}
}

void triColorLED::generateGrid()
{
	byte rgb = 0x00;
	byte led = 0x00;

	for (int iBit = 0; iBit < nMaxLEDs; iBit++)
	{
		led |= 1 << iBit; 

		for (int iColor = 0; iColor < nMaxColors; iColor++)
		{
			rgb |=  1 << (7 - iColor);
			stateGrid[iBit][iColor] = led + rgb;
			rgb = 0x00;
		}
		led = 0x00; 
	}
	
}

