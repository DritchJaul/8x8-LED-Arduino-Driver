// 
//
// This program drives a set of 8x8 LED matrix displays using several 74HC595 in serial.
// https://github.com/DritchJaul/8x8-LED-Arduino-Driver/
// 
// This program was written using the 788BS 8x8 LED matrix.
// Other displays may have different pinouts and may flip/mirror/mangle normal output.
//
//
//ST_CP
int latchPin = 8;
//SH_CP
int clockPin = 12;
//DS
int dataPin = 11;

// The number of 8x8 modules being used
const int numberOfModules = 3;

// Display buffer, this is directly outputed to the screen
byte disp[numberOfModules][8];

// Font Info
// Each letter in the font has the same width. This number can be changed to kern the characters to be farther or closer.
byte letterWidth = 6;
// This contains 95 letter images used in ascii from 0x20 to 0x7E (all non-control characters)
byte font[95][8] =  {{0,0,0,0,0,0,0,0},{0,4,4,4,4,4,0,4},{0,10,10,10,0,0,0,0},{0,0,10,31,10,31,10,0},{0,4,30,1,14,16,15,4},{0,2,21,10,4,10,21,8},{0,6,9,9,6,21,9,22},{0,4,4,4,0,0,0,0},{0,16,8,8,8,8,8,16},{0,4,8,8,8,8,8,4},{0,0,8,28,8,28,8,0},{0,0,8,8,62,8,8,0},{0,0,0,0,0,8,8,4},{0,0,0,0,62,0,0,0},{0,0,0,0,0,0,12,12},{0,16,8,12,4,6,2,1},{0,14,25,29,21,23,19,14},{0,4,6,4,4,4,4,14},{0,14,17,16,8,4,2,31},{0,14,17,16,14,16,17,14},{0,8,12,10,31,8,8,8},{0,31,1,1,15,16,17,14},{0,14,17,1,15,17,17,14},{0,31,16,8,4,4,2,2},{0,14,17,17,14,17,17,14},{0,14,17,17,30,16,17,14},{0,0,6,6,0,6,6,0},{0,0,6,6,0,4,6,2},{0,8,4,2,1,2,4,8},{0,0,0,31,0,31,0,0},{0,4,8,16,32,16,8,4},{0,14,17,16,12,4,0,4},{0,14,17,21,21,29,1,30},{0,14,17,17,31,17,17,17},{0,15,17,17,15,17,17,15},{0,14,17,1,1,1,17,14},{0,15,17,17,17,17,17,15},{0,31,1,1,15,1,1,31},{0,31,1,1,15,1,1,1},{0,14,17,1,25,17,17,14},{0,17,17,17,31,17,17,17},{0,31,4,4,4,4,4,31},{0,31,4,4,4,4,5,2},{0,17,9,5,3,5,9,17},{0,4,4,4,4,4,4,28},{0,17,27,21,17,17,17,17},{0,17,19,19,21,25,25,17},{0,14,17,17,17,17,17,14},{0,15,17,17,15,1,1,1},{0,14,17,17,17,21,25,30},{0,15,17,17,15,5,9,17},{0,14,17,1,14,16,17,14},{0,31,4,4,4,4,4,4},{0,17,17,17,17,17,17,14},{0,17,17,17,17,10,10,4},{0,17,17,17,17,21,27,17},{0,17,17,10,4,10,17,17},{0,17,17,10,4,4,4,4},{0,31,16,8,4,2,1,31},{0,14,2,2,2,2,2,14},{0,1,2,6,4,12,8,16},{0,14,8,8,8,8,8,12},{0,4,10,17,0,0,0,0},{0,0,0,0,0,0,0,31},{0,1,2,4,0,0,0,0},{0,0,0,14,16,30,17,30},{0,1,1,1,15,17,17,15},{0,0,0,14,17,1,17,14},{0,16,16,16,30,17,17,30},{0,0,0,14,17,31,1,14},{0,8,20,4,14,4,4,4},{0,0,0,30,17,30,16,14},{0,1,1,1,15,17,17,17},{0,0,0,4,0,4,4,4},{0,0,8,0,8,8,10,4},{0,1,1,9,5,3,5,9},{0,4,4,4,4,4,4,8},{0,0,0,11,21,21,21,21},{0,0,0,14,18,18,18,18},{0,0,0,14,17,17,17,14},{0,0,0,15,17,15,1,1},{0,0,0,30,17,30,16,16},{0,0,0,15,17,1,1,1},{0,0,0,30,1,14,16,15},{0,4,4,14,4,4,4,8},{0,0,0,17,17,17,17,30},{0,0,0,17,17,17,10,4},{0,0,0,17,21,21,21,10},{0,0,0,17,10,4,10,17},{0,0,0,17,17,30,16,14},{0,0,0,31,8,4,2,31},{0,24,4,4,2,4,4,24},{0,4,4,4,4,4,4,4},{0,6,8,8,16,8,8,6},{0,0,0,9,21,18,0,0}};
int asciiStart = 0x20;

// Text to be displayed. Special characters \n \0 \r \t etc unsupported
// Sample test: THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG the quick brown fox jumps over the lazy dog .,?! 0123456789 +-*/= (){}[]<> @#$%^&
char text[] = "Test!~";
// Speed of  the scroll. Smaller is faster. Use only multiples of 8 for small values
int cycleSpeed = 128;

void setup() {
  //Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
}


void loop() {
  // The current character being written to the display
  int currentChar = 0;
  while (1){
    cycleDisplayLeftByCharacter(text[currentChar], cycleSpeed); // Cycle the letter onto the display
    currentChar++;  // Move to the next letter
    if (currentChar == sizeof(text) - 1){ // Wrap around to the start of the string
      currentChar = 0;
    }
  }
}


// Shift onto the display a given character at a given speed
void cycleDisplayLeftByCharacter(byte character, int dispSpeed){
    // for each column in the selected letter
    for (int columnIndex = 0; columnIndex < letterWidth; columnIndex++){
      // shift each row of the display by 1 bit and shift in the letter column 
      for (int j = 0; j < 8; j++){
        // The display needs to be shifted by rows, so the high bit needs to be stored from each
        // byte to carry into the next byte. This is conveinent because it gives a simple place
        // to shift in the current letter.
        byte carry = ( getLetterPart(character,j) << ( 7 - columnIndex )) & 0x80; // Get current letter column and carry it into the first byte
        cycleDisplayRowLeft( j, carry ); //
      }
      writeToDisplay(dispSpeed);
    }
}


// Cycle individual rows in the display with a carry in.
void cycleDisplayRowLeft(byte row, byte carryIn){
    // Due to the way the display is oriented we shift in backwards from the display.
    // Otherwise the characters would be mirrored. This could be fixed by flipping the
    // font, but that would require reworking these sections also. Too tedious.
    for (int k = numberOfModules - 1; k >= 0; k--){
      byte carryOut = (disp[k][row] & 0x01) << 7; // Save high bit for carrying OUT
      disp[k][row] = disp[k][row] >> 1; // Shift byte
      disp[k][row] |= carryIn; // OR carry IN
      carryIn = carryOut; // make carry IN carry OUT
    } 
}

// Returns a letter row from the font for a given letter and row.
byte getLetterPart(byte letter, byte row){
  return font[letter - asciiStart][7 - row];
}

// Clears the display buffer
void clearDisplay(){
  for (int i = 0; i < numberOfModules; i++){
    for (int j = 0; j < 8; j++){
      disp[i][j] = 0;
    }
  }
}

// Physically activates the pins and interfaces with the hardware.
// Writes the entirety of the displayBuffer to the display for a given duration.
void writeToDisplay(int dispSpeed){
      for (int i = 0; i < dispSpeed; i++){
        digitalWrite(latchPin, LOW); // Unlock the shift register for shifting in data

        for (int j = numberOfModules; j >= 0; j--){
          shiftOut(dataPin, clockPin, MSBFIRST, disp[j][i % 8]); // Shift in all column data.
        }
        shiftOut(dataPin, clockPin, MSBFIRST, 1 << (i % 8)); // Shift in row data.
        digitalWrite(latchPin, HIGH);
      }
}


