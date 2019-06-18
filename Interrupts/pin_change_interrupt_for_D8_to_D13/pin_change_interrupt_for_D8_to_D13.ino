
volatile byte even=0;

ISR(PCINT0_vect) {
  if (even<=1){
    PORTB^=_BV(3);
  }

  if (even<=3 && even>1){
    PORTB^=_BV(0);
  }

  even++;
  if (even==4){
    even=0;
  }
}

void setup()
{
  pinMode(3,OUTPUT);
  pinMode(0,OUTPUT);
  GIMSK = _BV(PCIE);    // Enable pin change interrupt Table 9.3.2
  PCMSK = _BV(PCINT4);  // Enable the interrupt for only pin 4,Table 9.3.4
}

void loop ()   {
} //loop
