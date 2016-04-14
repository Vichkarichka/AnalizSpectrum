#include <SoftwareSerial.h>

#define COLS_NUMS 8
#define ROWS_NUMS 8

const int audioIn = A0;

SoftwareSerial serial(12, 13);

void setup() {
  serial.begin(9600);
  DDRD = 0xFF; //D-port as output. For selecting row
//  DDRB = B00000111; //Three last as outputs
}

void loop() {
  int audioVal = analogRead(audioIn) >> 7; //decreasing resolution to 3 (10 - 7) bits
  serial.println(audioVal);

  PORTD = ~((1 << audioVal) - 1);

  delay(100);
  
//  for(int col = 0; col < COLS_NUMS; col++) {
//    PORTB = col;
//    for(int row = 0; row < ROWS_NUMS; row++) {
//      PORTD = 1 << row;
//      delay(100);
//    }
//  }
}

inline int toRange(int oldMin, int oldMax, int newMin, int newMax, int val) {
  return ( (val - oldMin) / (oldMax - oldMin) ) * (newMax - newMin) + newMin;
}

unsigned int flp2(int x) {
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  return x - (x >> 1);
}

unsigned int clp2(int x) {
  x = x - 1;
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  return x + 1;
}

