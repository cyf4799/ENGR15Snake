#include <Adafruit_CircuitPlayground.h>
//Declare a 16 by 16 2D array of bools corresponding to the 16 by 16 board
bool board[16][16];

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
      //Initialize entire board as off
      board[row][column] = 0;
    }
  }
}

void loop() {
  //Set currentTime equal to millis
  currentTime = millis(); 
  
  //Update board for every defined interval of time
  if(currentTime - previousTime >= interval) {
    previousTime = currentTime; 


    //Clear board
    for(int row = 0; row < 16; row++) {
      for(int column = 0; column < 16; column++) {
        //Initialize entire board as off
        board[row][column] = 0;
      }
    }
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
  //2D array of int row and column coordinates of the snake 
  int location[256][2]; 
  //Integer variable pointing to the tail of the snake (pointer to the location 2D array)
  int tail; 
  //Integer variable pointing to the head of the snake (pointer to the location 2D array) 
  int head;

  //Constructor for Snake
  Snake() {
    //Set initial location of snake to be: (7, 6), (7, 7), (7, 8), (7, 9)
    location[0][0] = 7;
    location[0][1] = 6;
    location[1][0] = 7;
    location[1][1] = 7;
    location[2][0] = 7;
    location[2][1] = 8;
    location[3][0] = 7;
    location[3][1] = 9;

    //Set head and tail pointers 
    tail = 0;
    head = 3;   
  }
};

