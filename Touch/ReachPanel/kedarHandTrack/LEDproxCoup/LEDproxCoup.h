#ifndef LEDproxCoup_h
#define LEDproxCoup_h

#include <Arduino.h>

#define nMaxColors 3
#define nMaxLEDs 3

class LEDproxCoup
{
	public:
		void generateGrid();
		byte getState(byte);
		void Init();
		bool checkProx(byte,byte);
		
	private:
		byte stateGrid[nMaxLEDs][nMaxColors];
};
#endif