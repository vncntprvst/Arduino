#include "LEDproxCoup.h"

/*
triColorLED::triColorLED()
{
}

triColorLED::~triColorLED()
{
}
*/

void LEDproxCoup::Init()
{
	generateGrid();
	
	noInterrupts();

	// Initialize External Interrupts
	EICRA = 0x00;
	EICRB = 0x00;
	EIMSK = 0x00;
	PCICR = 0x00;
	PCMSK1 = 0x00;
	PCMSK2 = 0x00;
	PCMSK0 = 0x00;
	
	//Arm External INT0 with rising edge & INT1 with falling edge (We connect ORed REX to INT0 & ANDed Prox Sensors to INT1)
	//EICRA |= (1 << ISC01) | (1 << ISC00) | (1 << ISC11);
	//EIMSK |= (1 << INT0);//Enable INT0 for first run. We enable INT1 later.
	
	// Initialize Timer Registers for Ouput Compare	
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0x00;	
	TIMSK1 = 0x00;
	
	// Configuring Timer 1
	OCR1A = 0x3D09; // Output Compare Registers 1A
	// This number establishes the window time:
 	// 0x3D09 --> 15,625 in dec --> 0.5s
	// 0xFFFF --> 65,535 in dec --> 4.0s
	// 0x8C4A --> 35,914 in dec --> 2.4s
	
	TIMSK1 |= (1 << OCIE1A);
	TCCR1A |= (1 << COM1A0);
	//TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);	

	interrupts();
	
	// Initialize Data Direction Registers
	DDRB |= (1 << DDB5);
	DDRC = 0x00;
	DDRD = 0x00;
	DDRF = 0xff;
	DDRK = 0b11111000;
}

void LEDproxCoup::generateGrid()
{
	for (byte iBit = 0; iBit < nMaxLEDs; iBit++) // Change this to 3 or 5 years
	{
		for (byte iColor = 0; iColor < nMaxColors; iColor++) // nMaxColors is red, blue and green
		{
			stateGrid[iBit][iColor] |= (1 << iBit) | (1 << (7 - iColor));
		}
	}
}

byte LEDproxCoup::getState(byte LEDCommand)
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
	case 10:
		return stateGrid[3][0];
	case 11: 
		return stateGrid[3][1];
	case 12:
		return stateGrid[3][2];
	case 13:
		return stateGrid[4][0];
	case 14:
		return stateGrid[4][1];
	case 15:
		return stateGrid[4][2];
	default:
		return 0;
	}
}

bool LEDproxCoup::checkProx(byte LEDState,byte proxState)
{
	byte mask = 7;
	
	LEDState &= mask;
	proxState = (~proxState) & mask;
	
	if(proxState == LEDState)
		return true;
	else
		return false;
}

