#include <Adafruit_CircuitPlayground.h>

//Declare a 16 by 16 board
char board[16][16]; 
//Declare a 16 by 16 2D array of bools corresponding to the 16 by 16 board
bool boardStatus[16][16];

//Define a variable to store current elapsed time
unsigned long currentTime; 
//Define a variable to store time of last board update 
unsigned long previousTime = 0; 
//Define a constant for board update interval
const long interval = 1000;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  //Populate board and boardStatus
  for(int row = 0; row < 16; row++) {
    for(int column = 0; column < 16; column++) {
      //Black squares to represent when the pixel is off
      board[row][column] = '0';
      boardStatus[row][column] = 0; 
    }
  }
}

void loop() {
  //Set currentTime equal to millis
  currentTime = millis(); 
  
  //Update board for every defined interval of time
  if(currentTime - previousTime >= interval) {
    previousTime = currentTime; 

    //Print board to Serial
    for(int row = 0; row < 16; row++) {
      for(int column = 0; column < 16; column++) {
        Serial.print(board[row][column]);
      }
      Serial.println("");
    }
    Serial.println("");
  }
}

//Snake Class
class Snake 
{
  bool Xlocations[16*16];
  bool Ylocations[16*16];
}

