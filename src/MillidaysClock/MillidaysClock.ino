int latch=9;  //74HC595  pin 9 STCP
int clock=10; //74HC595  pin 10 SHCP
int data=8;   //74HC595  pin 8 DS
int digitSelectPins[] = { 2, 4, 3, 5 };
unsigned int displayDigits[] = { 16, 16, 16, 16 }; // blanks

unsigned char table[]= {
  0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
  0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,
  0x00 }; //Display digital data

void setup() {
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  for (size_t i = 0; i < sizeof(digitSelectPins) / sizeof(digitSelectPins[0]); ++i) {
    pinMode(digitSelectPins[i], OUTPUT);
  }
}

void selectDigit(unsigned int digit) {
  for (unsigned int i = 0; i < sizeof(digitSelectPins) / sizeof(digitSelectPins[0]); ++i) {
    digitalWrite(digitSelectPins[i], (i == digit) ? LOW : HIGH);
  }
}
/*   The most common method of using 74CH595
 *   latch->LOW : Begin transmitting signals.
 *   shiftOut(dataPin, clockPin, bitOrder, value)
 *   dataPin: the pin on which to output each bit. Allowed data types: int.
 *   clockPin: the pin to toggle once the dataPin has been set to the correct value. Allowed data types: int.
 *   bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit First, or, Least Significant Bit First).
 *   value: the data to shift out. Allowed data types: byte. 
 *   latch->HIch : The end of the transmission signal.
*/
void displayDigit(unsigned int digit, unsigned int num, bool addDecimalPoint) {
  unsigned char matrix = table[num] | (addDecimalPoint ? 0x80 : 0x00);
  digitalWrite(latch,LOW);
  shiftOut(data, clock, MSBFIRST, matrix);
  digitalWrite(latch,HIGH);
  selectDigit(digit);
  
}

void display(unsigned long duration) {
  unsigned long endTime = millis() + duration;
  while (millis() < endTime) {
    for (unsigned int i = 0; i < sizeof(digitSelectPins) / sizeof(digitSelectPins[0]); ++i) {
      displayDigit(i, displayDigits[i], i == 2);
      delay(1);
    }
  }
}

void setDigit(unsigned int digit, unsigned int val) {
  displayDigits[digit] = val;
}

void setDecimal(unsigned int num) {
  unsigned int numDigits = sizeof(digitSelectPins) / sizeof(digitSelectPins[0]);
  for (unsigned int i = 1; i <= numDigits; ++i) {
    unsigned int val = num % 10;
    setDigit(numDigits - i, val);
    num /= 10;
  }
}

void loop() {
  unsigned long hours = 13;
  unsigned long minutes = 21;
  unsigned long curTime = (hours * 60 + minutes) * 60 * 1000 + millis();
  unsigned long decamillidays = curTime * 10 / 60 / 60 / 24;
  setDecimal((unsigned int)decamillidays);
  display(500);
}
