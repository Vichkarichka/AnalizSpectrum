#include <SoftwareSerial.h>
#include "fix_fft.h"

#define COLS_NUMS 8
#define ROWS_NUMS 8

const int audioIn = A0;

const int N = 128;
const int LOG_2_N = 7;

char im[N], re[N];

//SoftwareSerial serial(A2, A3);

void setup() {
//  serial.begin(9600);
  DDRD = 0xFF; //D-port as output. For selecting row
  DDRB = B00011111; //Three last as outputs

  //for serial
//  pinMode(A2, INPUT);
//  pinMode(A3, OUTPUT);
}

void loop() {
  //collecting data
  for(int i = 0; i < N; i++) {
    re[i] = analogRead(audioIn);
    im[i] = 0;
  }
  //transform
  fix_fft(re, im, LOG_2_N, 0);

  //we interested only in absolute values
  for(int i = 0; i < 64; i++) {
    re[i] =  sqrt(re[i] * re[i] + im[i] * im[i]);
  }

  //collapsing to 32 columns of our display, so displayed bandwidth is from 0 to 16kHz
  for(int i = 0; i < 64; i+= 2) {
    //TODO: investigate why output range after fft has maximum value 30
    re[i] = map(re[i] + re[i+1], 0, 30, 0, 8); //calc avarage and remap value to 0 to 8 range.
  }

  for(int col = 0; col < 8; col++) {
    PORTB = col;
    PORTD = ~((1 << re[col]) - 1); //output col val
  }
  delay(15);
}

