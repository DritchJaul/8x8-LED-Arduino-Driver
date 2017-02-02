// 
//
// This program drives a set of 8x8 LED matrix displays using several 74HC595 in serial.
// https://github.com/DritchJaul/8x8-LED-Arduino-Driver/
// 
// This program was written using the 788BS 8x8 LED matrix.
// Other displays may have different pinouts and may flip/mirror/mangle normal output.
//
//

// Font Info
// Each letter in the font has the same width. This number can be changed to kern the characters to be farther or closer.
byte letterWidth = 6;
// This contains 95 letter images used in ascii from 0x20 to 0x7E (all non-control characters)
byte font[95][8] =  {{0,0,0,0,0,0,0,0},{0,0,190,0,0,0,0,0},{0,14,0,14,0,0,0,0},{40,124,40,124,40,0,0,0},{72,84,214,84,36,0,0,0},{68,42,84,168,68,0,0,0},{108,146,178,76,160,0,0,0},{0,0,14,0,0,0,0,0},{0,0,0,124,130,0,0,0},{0,0,130,124,0,0,0,0},{0,40,16,40,0,0,0,0},{0,16,16,124,16,16,0,0},{0,0,128,96,0,0,0,0},{0,16,16,16,16,16,0,0},{0,0,192,192,0,0,0,0},{128,96,56,12,2,0,0,0},{124,226,186,142,124,0,0,0},{0,132,254,128,0,0,0,0},{132,194,162,146,140,0,0,0},{68,146,146,146,108,0,0,0},{16,24,20,254,16,0,0,0},{94,146,146,146,98,0,0,0},{124,146,146,146,100,0,0,0},{2,194,50,10,6,0,0,0},{108,146,146,146,108,0,0,0},{76,146,146,146,124,0,0,0},{0,108,108,0,0,0,0,0},{0,204,108,0,0,0,0,0},{16,40,68,130,0,0,0,0},{40,40,40,40,40,0,0,0},{0,0,130,68,40,16,0,0},{4,2,178,18,12,0,0,0},{124,130,186,162,188,0,0,0},{252,18,18,18,252,0,0,0},{254,146,146,146,108,0,0,0},{124,130,130,130,68,0,0,0},{254,130,130,130,124,0,0,0},{254,146,146,146,130,0,0,0},{254,18,18,18,2,0,0,0},{124,130,130,146,116,0,0,0},{254,16,16,16,254,0,0,0},{130,130,254,130,130,0,0,0},{66,130,126,2,2,0,0,0},{254,16,40,68,130,0,0,0},{0,0,254,128,128,0,0,0},{254,4,8,4,254,0,0,0},{254,12,16,96,254,0,0,0},{124,130,130,130,124,0,0,0},{254,18,18,18,12,0,0,0},{124,130,162,194,252,0,0,0},{254,18,50,82,140,0,0,0},{76,146,146,146,100,0,0,0},{2,2,254,2,2,0,0,0},{126,128,128,128,126,0,0,0},{30,96,128,96,30,0,0,0},{254,64,32,64,254,0,0,0},{198,40,16,40,198,0,0,0},{6,8,240,8,6,0,0,0},{194,162,146,138,134,0,0,0},{0,254,130,130,0,0,0,0},{2,12,56,96,128,0,0,0},{0,2,130,254,0,0,0,0},{8,4,2,4,8,0,0,0},{128,128,128,128,128,0,0,0},{0,2,4,8,0,0,0,0},{64,168,168,168,240,0,0,0},{254,144,144,144,96,0,0,0},{112,136,136,136,80,0,0,0},{96,144,144,144,254,0,0,0},{112,168,168,168,48,0,0,0},{0,16,252,18,4,0,0,0},{16,168,168,168,120,0,0,0},{254,16,16,16,224,0,0,0},{0,0,232,0,0,0,0,0},{0,64,128,116,0,0,0,0},{254,32,80,136,0,0,0,0},{0,0,126,128,0,0,0,0},{248,8,240,8,240,0,0,0},{0,248,8,8,240,0,0,0},{112,136,136,136,112,0,0,0},{248,40,40,40,16,0,0,0},{16,40,40,40,248,0,0,0},{248,8,8,8,16,0,0,0},{144,168,168,168,72,0,0,0},{0,8,126,136,0,0,0,0},{120,128,128,128,248,0,0,0},{56,64,128,64,56,0,0,0},{120,128,112,128,120,0,0,0},{136,80,32,80,136,0,0,0},{24,160,160,160,120,0,0,0},{136,200,168,152,136,0,0,0},{0,16,108,130,130,0,0,0},{0,0,254,0,0,0,0,0},{0,130,130,108,16,0,0,0},{24,32,16,8,48,0,0,0}};
int asciiStart = 0x20;

// The number of 8x8 modules being used
const int numberOfModules = 3;

// Text to be displayed. Special characters \n \0 \r \t etc unsupported
// Sample test: THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG the quick brown fox jumps over the lazy dog .,?! 0123456789 +-*/= (){}[]<> @#$%^&
char text[] = "Display! ";
// Speed of  the scroll. Smaller is faster. Use only multiples of 8 for small values
int scrollSpeed = 64;

////////////////////////////////////////////////////////////////

// Display buffer, this is directly outputed to the screen
byte disp[numberOfModules][8];

//ST_CP
int latchPin = 8;
//SH_CP
int clockPin = 12;
//DS
int dataPin = 11;

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
    cycleDisplayLeftByCharacter(text[currentChar], scrollSpeed); // Cycle the letter onto the display
    currentChar++;  // Move to the next letter
    if (currentChar == sizeof(text) - 1){ // Wrap around to the start of the string
      currentChar = 0;
    }
  }
}


// Shift onto the display a given character at a given speed
void cycleDisplayLeftByCharacter(byte character, int scrollSpeed){
    // for each column in the selected letter
    for (byte columnIndex = 0; columnIndex < letterWidth; columnIndex++){

      byte carry = font[character - asciiStart][columnIndex];
      for (byte i = numberOfModules - 1; i != 255; i--){
        for (byte j = 7; j != 255; j--){
          byte carryOut = disp[i][j];
          disp[i][j] = carry;
          carry = carryOut;
        }
      }

      writeToDisplay(scrollSpeed);
    }
}


// Physically activates the pins and interfaces with the hardware.
// Writes the entirety of the displayBuffer to the display for a given duration.
void writeToDisplay(int dispSpeed){
      while (dispSpeed >= 0){
        byte mod =  dispSpeed % 8;
        digitalWrite(latchPin, LOW); // Unlock the shift register for shifting in data

        for (byte j = numberOfModules; j != 255; j--){
          shiftOut(dataPin, clockPin, MSBFIRST, disp[j][mod]); // Shift in all column data.
        }
        shiftOut(dataPin, clockPin, MSBFIRST, 1 << (mod)); // Shift in row data.
        digitalWrite(latchPin, HIGH);
        dispSpeed--;
      }
}


