
//ST_CP
int latchPin = 8;

//SH_CP
int clockPin = 12;

//DS
int dataPin = 11;

// This is for an 8x7seg display
//byte nums[11] = {111,6,91,31,54,61,125,7,127,55,0};


byte letters[26][8] =  {{24,36,66,66,126,66,66,66},
                        {62,66,66,62,66,66,66,62},
                        {60,66,2,2,2,2,66,60},
                        {30,34,66,66,66,66,34,30},
                        {126,2,2,62,2,2,2,126},
                        {126,2,2,62,2,2,2,2},
                        {60,66,2,2,114,66,66,60},
                        {66,66,66,126,66,66,66,66},
                        {62,8,8,8,8,8,8,62},
                        {124,16,16,16,16,16,18,12},
                        {34,18,10,6,6,10,18,34},
                        {2,2,2,2,2,2,2,126},
                        {66,102,90,66,66,66,66,66},
                        {66,70,74,74,82,82,98,66},
                        {60,66,66,66,66,66,66,60},
                        {62,66,66,66,62,2,2,2},
                        {60,66,66,66,82,98,66,188},
                        {62,66,66,66,62,18,34,66},
                        {60,66,2,2,60,64,66,60},
                        {62,8,8,8,8,8,8,8},
                        {66,66,66,66,66,66,66,60},
                        {66,66,66,66,36,36,24,24},
                        {66,66,66,66,66,90,102,66},
                        {66,66,36,24,24,36,66,66},
                        {66,66,66,124,64,64,64,60},
                        {126,64,32,16,8,4,2,126}};


char text[] = "HELLO WORLD";
unsigned short disp[8] = {0,0,0,0,0,0,0,0};

void setup() {
  //set pins to output so you can control the shift register
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 8; i++){
    disp[i] &= 0x00FF;
  }
  for (int i = 0; i < sizeof(text); i++){
    for (int j = 0; j < 8; j++){
      for (int k = 0; k < 128; k++){
        writeToDisplay( 1 << (k % 8), (byte)(disp[k % 8] ) );
      }
      for (int k = 0; k < 8; k++){
        disp[k] = disp[k] >> 1;
      }
    }
    for (int j = 0; j < 8; j++){
      if (32 != text[i]){
        disp[j] |= (letters[text[i] - 65][j] << 8);
      }else{
        disp[j] &= 0x00FF;
      }
    }
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



    


