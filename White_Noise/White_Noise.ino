  /**
 * references:
 * white-noise generation: https://code.google.com/p/greennoise/downloads/detail?name=prbsGen.pde&can=2&q=label%3AWhite+Noise
 * sleepduino: http://gareth.halfacree.co.uk/2012/02/introducing-the-sleepduino
 */
 
//  pin setup
const int speakerPin = 9;

//  for noise generation
unsigned long int reg;

  void setup() {
  pinMode(speakerPin,OUTPUT);
  // Arbitrary inital value; must not be zero
  reg = 0x55aa55aaL; //The seed for the bitstream. It can be anything except 0.
}

void loop() {
//  for (int i=0; i<255; i++) { // this is to test speakers 
//    analogWrite(speakerPin,i);
//  delay(10);
//  }
//    int potValue = analogRead(potPin);  //  0 to 1023
    int potValue = 50;
    int frequency = map(potValue, 0, 1023, 0, 1000);
    generateNoise(frequency);
}

void generateNoise(int frequency) {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
   
  // Extract four chosen bits from the 32-bit register
  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;

  // EXOR the four bits together
  lobit = b31 ^ b29 ^ b25 ^ b24;
  
  // Shift and incorporate new bit at bit position 0
  newr = (reg << 1) | lobit;
  
  // Replace register with new value
  reg = newr;
  
  // Drive speaker pin from bit 0 of 'reg'
  digitalWrite(speakerPin, reg & 1);
  
  // Delay (50) corresponds to 20kHz, but the actual frequency of updates
  // will be lower, due to computation time and loop overhead
  delayMicroseconds(frequency);    // Changing this value changes the frequency.
}
   
