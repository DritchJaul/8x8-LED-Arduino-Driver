
//ST_CP
int latchPin = 8;
//SH_CP
int clockPin = 12;
//DS
int dataPin = 11;
const int numberOfModules = 3;

// Font Info
byte letterWidth = 6;
byte letters[127][8] =  {{0,0,0,0,0,0,0,0},{0,4,4,4,4,4,0,4},{0,10,10,10,0,0,0,0},{0,0,10,31,10,31,10,0},{0,4,30,1,14,16,15,4},{0,2,21,10,4,10,21,8},{0,6,9,9,6,21,9,22},{0,4,4,4,0,0,0,0},{0,16,8,8,8,8,8,16},{0,4,8,8,8,8,8,4},{0,0,8,28,8,28,8,0},{0,0,8,8,62,8,8,0},{0,0,0,0,0,8,8,4},{0,0,0,0,62,0,0,0},{0,0,0,0,0,0,12,12},{0,16,8,12,4,6,2,1},{0,14,25,29,21,23,19,14},{0,4,6,4,4,4,4,14},{0,14,17,16,8,4,2,31},{0,14,17,16,14,16,17,14},{0,8,12,10,31,8,8,8},{0,31,1,1,15,16,17,14},{0,14,17,1,15,17,17,14},{0,31,16,8,4,4,2,2},{0,14,17,17,14,17,17,14},{0,14,17,17,30,16,17,14},{0,0,6,6,0,6,6,0},{0,0,6,6,0,4,6,2},{0,8,4,2,1,2,4,8},{0,0,0,31,0,31,0,0},{0,4,8,16,32,16,8,4},{0,14,17,16,12,4,0,4},{0,14,17,21,21,29,1,30},{0,14,17,17,31,17,17,17},{0,15,17,17,15,17,17,15},{0,14,17,1,1,1,17,14},{0,15,17,17,17,17,17,15},{0,31,1,1,15,1,1,31},{0,31,1,1,15,1,1,1},{0,14,17,1,25,17,17,14},{0,17,17,17,31,17,17,17},{0,31,4,4,4,4,4,31},{0,31,4,4,4,4,5,2},{0,17,9,5,3,5,9,17},{0,4,4,4,4,4,4,28},{0,17,27,21,17,17,17,17},{0,17,19,19,21,25,25,17},{0,14,17,17,17,17,17,14},{0,15,17,17,15,1,1,1},{0,14,17,17,17,21,25,30},{0,15,17,17,15,5,9,17},{0,14,17,1,14,16,17,14},{0,31,4,4,4,4,4,4},{0,17,17,17,17,17,17,14},{0,17,17,17,17,10,10,4},{0,17,17,17,17,21,27,17},{0,17,17,10,4,10,17,17},{0,17,17,10,4,4,4,4},{0,31,16,8,4,2,1,31},{0,14,2,2,2,2,2,14},{0,1,2,6,4,12,8,16},{0,14,8,8,8,8,8,12},{0,4,10,17,0,0,0,0},{0,0,0,0,0,0,0,31},{0,1,2,4,0,0,0,0},{0,0,0,14,16,30,17,30},{0,1,1,1,15,17,17,15},{0,0,0,14,17,1,17,14},{0,16,16,16,30,17,17,30},{0,0,0,14,17,31,1,14},{0,8,20,4,14,4,4,4},{0,0,0,30,17,30,16,14},{0,1,1,1,15,17,17,17},{0,0,0,4,0,4,4,4},{0,0,8,0,8,8,10,4},{0,1,1,9,5,3,5,9},{0,4,4,4,4,4,4,8},{0,0,0,11,21,21,21,21},{0,0,0,14,18,18,18,18},{0,0,0,14,17,17,17,14},{0,0,0,15,17,15,1,1},{0,0,0,30,17,30,16,16},{0,0,0,15,17,1,1,1},{0,0,0,30,1,14,16,15},{0,4,4,14,4,4,4,8},{0,0,0,17,17,17,17,30},{0,0,0,17,17,17,10,4},{0,0,0,17,21,21,21,10},{0,0,0,17,10,4,10,17},{0,0,0,17,17,30,16,14},{0,0,0,31,8,4,2,31},{0,24,4,4,2,4,4,24},{0,4,4,4,4,4,4,4},{0,6,8,8,16,8,8,6},{0,0,0,9,21,18,0,0}};
char text[] = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG the quick brown fox jumps over the lazy dog .,?! 0123456789 +-*/= (){}[]<> @#$%^&";

int cycleSpeed = 256;
byte disp[numberOfModules][8];

void setup() {
  //set pins to output so you can control the shift register
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
}

void loop() {
  
  
  clearDisplay();
  for (int i = 0; i < sizeof(text) - 1; i++){
    cycleDisplayLeftByCharacter(text[i], cycleSpeed);
  }
  for (int i = 0; i < (numberOfModules * 8) - letterWidth; i++){
    cycleDisplayLeftByCharacter(' ', cycleSpeed);
  }
}


void cycleDisplayLeftByCharacter(byte character, int dispSpeed){
    for (int m = 0; m < letterWidth; m++){
      for (int j = 0; j < 8; j++){
        byte carry = ( getLetterPart(character,j) << ( 7 - m)) & 0x80;
        cycleDisplayRowLeft( j, carry );
      }
      writeToDisplay(dispSpeed);
    }
}



void cycleDisplayRowLeft(byte row, byte carryIn){
    for (int k = numberOfModules - 1; k >= 0; k--){
      byte carryOut = (disp[k][row] & 0x01) << 7;
      disp[k][row] = disp[k][row] >> 1;
      disp[k][row] |= carryIn;
      carryIn = carryOut;
    } 
}

byte getLetterPart(byte letter, byte row){

  return letters[letter - 32][7 - row];
}

void clearDisplay(){
  for (int i = 0; i < numberOfModules; i++){
    for (int j = 0; j < 8; j++){
      disp[i][j] = 0;
    }
  }
}

void writeToDisplay(int dispSpeed){
      for (int i = 0; i < dispSpeed; i++){
        digitalWrite(latchPin, LOW);

        for (int j = 0; j < numberOfModules; j++){
          shiftOut(dataPin, clockPin, MSBFIRST, disp[j][i % 8]);
        }
        shiftOut(dataPin, clockPin, MSBFIRST, 1 << (i % 8));
        digitalWrite(latchPin, HIGH);
      }
}


