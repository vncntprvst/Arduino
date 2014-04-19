#ifndef triColorLED_h
#define triColorLED_h

#include <Arduino.h>

#define nMaxColors 3
#define nMaxLEDs 3

class triColorLED
{
	public:
		void generateGrid();
		byte getState(byte);
		
	private:
		byte stateGrid[nMaxLEDs][nMaxColors];
};
#endif