#include "TimerOne.h"
#include "fix_fft.h"

#define BLOCKS_NUM 4
#define COLS_IN_BLOCK_NUM 8
#define COLS_NUMS BLOCKS_NUM * COLS_IN_BLOCK_NUM
#define ROWS_NUM 8
#define COL_REFRESH_PERIOD 16666

const int audioIn = A2;

const int N = 128;
const int LOG_2_N = 7;

char im[N], re[N];
char renderBuffer[COLS_NUMS] = {
  1, 2, 3, 4, 5, 6, 7, 8,
  1, 2, 3, 4, 5, 6, 7, 8,
  1, 2, 3, 4, 5, 6, 7, 8,
  1, 2, 3, 4, 5, 6, 7, 8
};

void checkDisplay() {
  for(unsigned char row = 0; row < ROWS_NUM; row++) {
    for(unsigned char activeBlock = 0; activeBlock < BLOCKS_NUM; activeBlock++) {
      switch(activeBlock) {
        case 0:
          digitalWrite(A0, LOW);
          digitalWrite(A1, LOW);
          break;
        case 1:
          digitalWrite(A0, HIGH);
          digitalWrite(A1, LOW);
          break;
        case 2:
          digitalWrite(A0, LOW);
          digitalWrite(A1, HIGH);
          break;
        case 3:
          digitalWrite(A0, HIGH);
          digitalWrite(A1, HIGH);
          break;
      }
      for(unsigned char col = 0; col < COLS_IN_BLOCK_NUM*2; col++) { // WTF?????
          PORTB = col << 3;  //WTF???????
          PORTD = 1 << row;
          delay(25);
      }
    }
  }
}

void setup() {
  DDRD = 0xFF; //D-port as output. For selecting row
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  //DDRC = B00000011; //Block select WHY NO?!?!???!?

  Timer1.initialize(COL_REFRESH_PERIOD);
  Timer1.attachInterrupt(renderCol);

  checkDisplay();
}

void loop() {
  //collecting data
//  for(int i = 0; i < N; i++) {
//    re[i] = analogRead(audioIn);
//    im[i] = 0;
//  }
//  //transform
//  fix_fft(re, im, LOG_2_N, 0);
//
//  //we interested only in absolute values
//  for(int i = 0; i < 64; i++) {
//    re[i] =  sqrt(re[i] * re[i] + im[i] * im[i]);
//  }

  //collapsing to 32 columns of our display, so displayed bandwidth is from 0 to 16kHz
//  for(int i = 0; i < 64; i+= 2) {
//    //TODO: investigate why output range after fft has maximum value 30
//    renderBuffer[i] = map(re[i] + re[i+1], 0, 1024, 0, 8); //calc avarage and remap value to 0 to 8 range.
//  }
}

unsigned char activeBlock = 0;
unsigned char activeCol = 0;

void renderCol() {
//  if(activeCol > COLS_IN_BLOCK_NUM - 1) {
//    if(activeBlock > BLOCKS_NUM - 1) {
//      activeBlock = 0;
//    }
//    PORTC = activeBlock++;
//    activeCol = 0;
//  }
//  
//  PORTB = activeCol;
//  PORTD = ((1 << renderBuffer[activeBlock + activeCol++]) - 1); //output col val
}

