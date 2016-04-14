#define COLS_NUMS 8
#define ROWS_NUMS 8

void setup() {
  // put your setup code here, to run once:
  // put your main code here, to run repeatedly:
  DDRD = 0xFF; //D-port as output. For selecting row
  DDRB = B00000111; //Three last as outputs
}

void loop() {
  for(int col = 0; col < COLS_NUMS; col++) {
    PORTB = col;
    for(int row = 0; row < ROWS_NUMS; row++) {
      PORTD = 1 << row;
      delay(100);
    }
  }
}
