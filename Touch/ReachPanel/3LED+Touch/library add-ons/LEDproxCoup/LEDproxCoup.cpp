#include "LEDproxCoup.h"
//LED and Proximity Couple module Class
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
	
	//Initialise Timer Registers for Output Compare	
	TCCR1A = 0x00; // Timer/Counter Control Register 1A
	TCCR1B = 0x00; // Timer/Counter Control Register 1B
	TCNT1 = 0x00;	// Timer/Counter 1 
	TIMSK1 = 0x00; // Timer/Counter 1 Interrupt Mask Register
	
	//Configuring Timer1
	OCR1A = 0xFFFF; // Output Compare Registers 1A
	//This number establishes the window time:
 	// 0x3D09 --> 15,625 in dec --> 0.5s
	// 0xFFFF --> 65,535 in dec --> 4.0s
	// 0x8C4A --> 35,914 in dec --> 2.4s
	
	TIMSK1 |= (1 << OCIE1A);
	TCCR1A |= (1 << COM1A0);
	
	interrupts();
	
	//Initialise Data Direction Registers
	DDRB |= (1 << DDB5);//To check timer output connect O'scope to PortB pin5
	DDRA = 0b11110000;//PORTA pins 0:3 --> Input (from Breakout Board) | 4:7 --> Output (pin4 for Reward Feedback to Breakout Board)
	DDRF = 0xff;//PORTF all pins --> Output (To LEDs)
	DDRK = 0b11111000;//PORTK pins 0:2 --> Input (From proxSensors)
}

//Generating Grid which contains Bit patterns for lighting specific LED with specific color
void LEDproxCoup::generateGrid()
{
	for (byte iBit = 0; iBit < nMaxLEDs; iBit++)
		for (byte iColor = 0; iColor < nMaxColors; iColor++)
			stateGrid[iBit][iColor] |= (1 << iBit) | (1 << (7 - iColor));
}

byte LEDproxCoup::getState(byte LEDCommand)
{
	//First Index of array corresponds to LED number
	//Second Index of array corresponds to Color:
	//0:Red | 1:Green | 2:Blue
	//Add more cases after adding more modules
	switch(LEDCommand)
	{
	case 1://LED0 Red
		return stateGrid[0][0];
	case 2://LED0 Green 
		return stateGrid[0][1];
	case 3://LED0 Blue
		return stateGrid[0][2];
	case 4://LED1 Red
		return stateGrid[1][0];
	case 5://LED1 Green
		return stateGrid[1][1];
	case 6://LED1 Blue	
		return stateGrid[1][2];
	case 7://LED2 Red
		return stateGrid[2][0];
	case 8://LED2 Green
		return stateGrid[2][1];
	case 9://LED2 Blue
		return stateGrid[2][2];
	default:
		return 0;
	}
}

//Method to check if the correct proximity sensor is activated
bool LEDproxCoup::checkProx(byte LEDState,byte proxState)
{
	byte mask = 7;
	bool result;
	
	LEDState &= mask;
	proxState = (~proxState) & mask;
	
	result = (proxState == LEDState) ? true : false;
	
	return result;
}

