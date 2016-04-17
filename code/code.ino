#include "TimerOne.h"
#include "fix_fft.h"

#define BLOCKS_NUM 4
#define COLS_IN_BLOCK_NUM 8
#define COLS_NUMS BLOCKS_NUM * COLS_IN_BLOCK_NUM
#define ROWS_NUM 8
#define COL_REFRESH_PERIOD 16666 / COLS_NUMS

const int audioIn = A3;

const int N = 128;
const int LOG_2_N = 7;

const unsigned char patterns[ROWS_NUM + 1] = {
  B00000000,
  B00000001,
  B00000011,
  B00000111,
  B00001111,
  B00011111,
  B00111111,
  B01111111,
  B11111111
};

char im[N], re[N];
char renderBuffer[COLS_NUMS] = {
  1,2,3,4,5,6,7,8,
  1,2,3,4,5,6,7,8,
  1,2,3,4,5,6,7,8,
  1,2,3,4,5,6,7,8
};

inline void setCol(unsigned int col) {
  digitalWrite(8, col &   B00000001);
  digitalWrite(9, col &   B00000010);
  digitalWrite(10, col &  B00000100);
}

inline void setBlock(unsigned int block) {
  digitalWrite(A0, block & B00000001);
  digitalWrite(A1, block & B00000010);
}

inline void setColData(unsigned int rowData) {
  PORTD = rowData;
  digitalWrite(16, rowData & B00100000);
  digitalWrite(14, rowData & B01000000);
}

void checkDisplay() {
  for(unsigned char row = 0; row < ROWS_NUM; row++) {
    for(unsigned char activeBlock = 0; activeBlock < BLOCKS_NUM; activeBlock++) {
          setBlock(activeBlock);
      for(unsigned char col = 0; col < COLS_IN_BLOCK_NUM; col++) {
          setCol(col);
          setColData(1 << row);
          delay(5);
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
  pinMode(audioIn, INPUT);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
  //DDRC = B00000011; //Block select WHY NO?!?!???!?

  checkDisplay();

  Timer1.initialize(COL_REFRESH_PERIOD);
  Timer1.attachInterrupt(renderCol);
  
  delay(5000);  //initial value will be shown as a sample
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
    renderBuffer[i] = map(re[i] + re[i+1], 0, 32, 0, ROWS_NUM); //calc avarage and remap value to 0 to 8 range.
  }
}

unsigned char activeBlock = 0;
unsigned char activeCol = 0;

void renderCol() {
  if(activeCol == COLS_IN_BLOCK_NUM) {
    activeCol = 0;
    
    if(activeBlock == BLOCKS_NUM) {
      activeBlock = 0;
    }

    setBlock(activeBlock);
    activeBlock++;
  }
  setCol(activeCol);
  setColData(patterns[renderBuffer[activeBlock + activeCol]]);
  activeCol++;
}

