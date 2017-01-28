
//ST_CP
int latchPin = 8;

//SH_CP
int clockPin = 12;

//DS
int dataPin = 11;

// This is for an 8x7seg display
//byte nums[11] = {111,6,91,31,54,61,125,7,127,55,0};

void setup() {
  //set pins to output so you can control the shift register
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  spiral();
}


// Draw a dot that spirals towards then away from the center.
void spiral(){
  int offVel = 1;
  int xOff = 0;
  int yOff = 0;
  int x = 0;
  int y = 0;
  int xVel = 1;
  int  yVel = 0;

  while (1){
    writeToDisplay(1 << y, 1 << x);
    if (x + xOff + xVel > 7){
      yVel = 1;
      xVel = 0;
    }
    if (y + yOff + yVel > 7){
      yVel = 0;
      xVel = -1;
    }
    if (x - xOff + xVel < 0){
      yVel = -1;
      xVel = 0;
      yOff += offVel;
    }
    if (y - yOff + yVel < 0){
      yVel = 0;
      xVel = 1;
      xOff += offVel;
    }
    if (xOff > 2){
      
      offVel = -1;
      
    }
    if (xOff < 1){
      offVel = 1;
    }

    x += xVel;
    y += yVel;
    delay(15);
  }
}


void writeToDisplay(byte row, byte column){
      digitalWrite(latchPin, LOW);

      // The two 8bit registers are in series. First is the Colum, then the Row.
      //The Row needs to be written to first because it needs to be pushed through the Column register
      shiftOut(dataPin, clockPin, MSBFIRST, row);
      shiftOut(dataPin, clockPin, MSBFIRST, column);

      digitalWrite(latchPin, HIGH);
}



    


