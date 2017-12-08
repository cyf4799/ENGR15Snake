// Include libraries for LED Matrix control
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// Create LED Matrices: 2 16x8 matrices 
// matrix1: Left matrix
// matrix2: Right matrix 
Adafruit_8x16minimatrix matrix1 = Adafruit_8x16minimatrix();
Adafruit_8x16minimatrix matrix2 = Adafruit_8x16minimatrix();

// Configure Arduino pin numbers for joy-stick control
// Digital pin connected to switch output 
const int JOYSTICK_SW_pin = 8; 
// Analog pin connected to X output
const int JOYSTICK_X_pin = 0; 
// Analog pin connected to Y output
const int JOYSTICK_Y_pin = 1; 

// Classes: Snake, Pellet 
// Snake Class
class Snake
{
  // Private states for Snake class 
  // Variables of the Snake class are encapsulated 
  private:
    // 2D array of row and column coordinates of the snake
    // Locations will be stored in a circular array, which will wrap around whenever the capcity is exceeded
    int location[256][2];
    // Integer variable pointing to the tail of the snake (pointer to the location 2D array)
    int tail;
    // Integer variable pointing to the head of the snake (pointer to the location 2D array)
    int head;
    // Character variable specifying snake direction
    char directionSnake;

  // Public behaviors for Snake class
  // Methods for augmenting the Snake class 
  public:
    // Constructor for Snake class
    Snake() {
      // Set initial location of snake to be: (7, 6), (7, 7), (7, 8), (7, 9)
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

      //Set direction to right
      directionSnake = 'r';
    }
    // Accessor method for tail variable 
    int getTail() {
      return tail;
    }
    // Accessor method for head variable
    int getHead() {
      return head;
    }
    // Accessor method for row location of index x
    int getLocationRow(int x) {
      return location[x][0];
    }
    // Accessor method for column location of index y
    int getLocationColumn(int y) {
      return location[y][1];
    }
    // Accessor method for direction variable 
    char getDirection() {
      return directionSnake;
    }
    // Mutator method for direction: Changing snake direction 
    setDirection(char dir) {
      // Conditional to check if input is valid 
      if (dir == 'u' || dir == 'd' || dir == 'l' || dir == 'r') {
        directionSnake = dir;
      }
    }
    // Method to update location snake 
    // Returns a boolean specifying whether the location update is valid
    bool newLocation() {
      //  Create a outOfBounds boolean to be returned
      bool outOfBounds = false;
      // Save previous head and tail locations
      int prevHead = head;
      int prevTail = tail;
      // Set new head location
      head = head + 1;
      // Set new tail location
      tail = tail + 1;
      // Wrap around the circular array if head > 255 or tail > 255
      if (head > 255) {
        head = 0;
      }
      if (tail > 255) {
        tail = 0;
      }
      // Get previous location
      int newRow = getLocationRow(prevHead);
      int newColumn = getLocationColumn(prevHead);
      // Update location according to direction
      switch (directionSnake) {
        case 'u':
          newRow = newRow - 1;
          break;
        case 'd':
          newRow = newRow + 1;
          break;
        case 'l':
          newColumn = newColumn - 1;
          break;
        case 'r':
          newColumn = newColumn + 1;
          break;
      }
      // Check for out of bounds. Reset if out of bounds.
      if (newRow >= 16) {
        newRow = newRow - 1;
        outOfBounds = true;
      }
      if (newRow < 0) {
        newRow = newRow + 1;
        outOfBounds = true;
      }
      if (newColumn >= 16) {
        newColumn = newColumn - 1;
        outOfBounds = true;
      }
      if (newColumn < 0) {
        newColumn = newColumn + 1;
        outOfBounds = true;
      }
      // Update new head location
      location[head][0] = newRow;
      location[head][1] = newColumn;
      return outOfBounds;
    }
    // Method for eating Pellets:
    // Increase length by changing head node without changing the tail node
    bool eatPellet() {
      // Use Snake's newLocation() method but reset the tail node
      bool outOfBounds = newLocation();
      // Set tail pointer back to previous location
      tail = tail - 1;
      // If tail index is less than 1, wrap around circular array 
      if (tail < 0) {
        tail = 255;
      }
    }
};

// Pellet Class
class Pellet
{
  // Private states of the Pellet Class
  // Variables are encapsulated
  private:
    // Define an integer variable for the row coordinate of the pellet
    int rowLocation;
    // Define an integer variable for the column coordinate of the pellet
    int columnLocation;
  // Public behaviors of the Pellet Class
  // Methods 
  public:
    // Constructor method
    Pellet() {
      //Set initial location
      randomizeLocation();
    }
    // Accessor for getting the row of the pellet
    int getRow() {
      return rowLocation;
    }
    // Accessor for getting the column of the pellet
    int getColumn() {
      return columnLocation;
    }
    // Method for randomizing location
    randomizeLocation() {
      //Use of Arduino library random function to generate a number between 0 and 15 
      rowLocation = random(0, 16);
      columnLocation = random(0, 16);
    }
};

// Declare a 16 by 16 2D array of bools corresponding to the 16 by 16 board
bool board[16][16];
// Define a variable to store current elapsed time
unsigned long currentTime;
// Define a variable to store time of last board update
unsigned long previousTime = 0;
// Define a constant for board update interval
const long interval = 50;

// Splash Screens: Start and Game Over 
static const uint8_t PROGMEM
game_over_matrix_1[] =
{ B00000000,
  B01110111,
  B01000101,
  B01010111,
  B01010101,
  B01110101,
  B00000000,
  B00000000,
  B00000000,
  B01110101,
  B01010101,
  B01010101,
  B01010101,
  B01110010,
  B00000000,
  B00000000
},
game_over_matrix_2[] =
{ B00000000,
  B01010110,
  B01110100,
  B01010110,
  B01010100,
  B01010110,
  B00000000,
  B00000000,
  B00000000,
  B01101110,
  B01001010,
  B01101110,
  B01001100,
  B01101010,
  B00000000,
  B00000000
},
start_matrix_1[] = 
{ B00000000, 
  B01110100,
  B01000110,
  B01110101,
  B00010100,
  B01110100,
  B01000000,
  B01000111,
  B01111100,
  B00000000,
  B01110111,
  B00100101,
  B00100101,
  B00100110,
  B00100101,
  B00000000
},
start_matrix_2[] = 
{
  B00000000,
  B01011110,
  B01010010,
  B01011110,
  B11010010,
  B01010010,
  B00000000,
  B11111010,
  B00000000,
  B00000000,
  B01110101,
  B00100101,
  B00100010,
  B00100101,
  B01110101,
  B00000000
};

// Define a snake object
Snake snake;
// Define a pellet object
Pellet pellet;
// Define a boolean to check whether to continue the game
bool runGame;
// Define a boolean to check whether to bring up start screen splash screen
bool startScreen; 
// Define a boolean to check whether to bring up game over splash screen 
bool gameOverScreen;
// Define a boolean to check if a direction change is allowed
bool directionChangeAllowed;

// Arduino setup function 
void setup() {
  // Start serial monitor: For debugging 
  Serial.begin(9600);
  
  // Call setUpBoard() function: Defined below loop
  setUpBoard();
  
  // Start by bringing up the start game splash screen
  // Set runGame to be false
  runGame = false;
  // Set startScreen to true
  startScreen = true;
  // Set gameOverScreen to false 
  gameOverScreen = false; 
  
  // Setup for Joystick switch input
  pinMode(JOYSTICK_SW_pin, INPUT);
  digitalWrite(JOYSTICK_SW_pin, LOW);

  // Setup for LED Matrices
  matrix1.begin(0x70);
  matrix2.begin(0x72);

  // Initialize direction change allowed to true
  directionChangeAllowed = true;
}

void loop() {
  
  // If the startScreen boolean is TRUE 
  if (startScreen) { 
    // Display start game splash screen 
    matrix1.clear();
    matrix2.clear();
    matrix1.drawBitmap(0, 0, start_matrix_1, 8, 16, 1);
    matrix2.drawBitmap(0, 0, start_matrix_2, 8, 16, 1);
    matrix1.writeDisplay();
    matrix2.writeDisplay();

    // Check if user moves the joystick 
    // If joystick is moved, change startScreen to false and runGame to true 
    if((analogRead(JOYSTICK_Y_pin) < 100 || analogRead(JOYSTICK_Y_pin) > 900 || analogRead(JOYSTICK_X_pin) < 100 || analogRead(JOYSTICK_X_pin) > 900) && runGame == false) { 
      matrix1.clear();
      matrix2.clear();
      startScreen = false;
      runGame = true; 
    }
  }

  // While the runGame boolean is TRUE 
  while (runGame) {
    // Set currentTime equal to millis
    currentTime = millis();

    // Control using Joystick
    int YAxisofJoyStick = analogRead(JOYSTICK_Y_pin);
    int XAxisofJoyStick = analogRead(JOYSTICK_X_pin);
    // Get the current direction of the snake 
    char snakeCurDirection = snake.getDirection();
    // Create a char variable to store future direction of snake 
    // The snake's direction will be updated upon frame change on the LED matrix
    char snakeFutureDirection;
    // According to user input on the joy-stick, the snakeFutureDirection variable will be augmented 
    // the directionChangeAllowed variable will be changed to false to prevent direction changes between LED frame updates 
    if (YAxisofJoyStick > 1000 && directionChangeAllowed) {
      if (snakeCurDirection == 'l' || snakeCurDirection == 'r') {
        // The snake can only go up if it is on a horizontal path
        snakeFutureDirection = 'u';
        directionChangeAllowed = false;
      }
    }
    if (YAxisofJoyStick < 100 && directionChangeAllowed) {
      // The snake can only go down if it is on a horizontal path
      if (snakeCurDirection == 'l' || snakeCurDirection == 'r') {
        snakeFutureDirection = 'd';
        directionChangeAllowed = false;
      }
    }
    if (XAxisofJoyStick > 1000 && directionChangeAllowed) {
      // The snake can only go right if it is on a vertical path
      if (snakeCurDirection == 'u' || snakeCurDirection == 'd') {
        snakeFutureDirection = 'r';
        directionChangeAllowed = false;
      }
    }
    if (XAxisofJoyStick < 100 && directionChangeAllowed) {
      // The snake can only go left if it is on a vertical path
      if (snakeCurDirection == 'u' || snakeCurDirection == 'd') {
        snakeFutureDirection = 'l';
        directionChangeAllowed = false;
      }
    }

    // Debounce 
    delayMicroseconds(50000);

    // Update board for every defined interval of time
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;


      // Clear board
      for (int row = 0; row < 16; row++) {
        for (int column = 0; column < 16; column++) {
          board[row][column] = 0;
        }
      }

      // Update Snake location on Board
      for (int coordinate = snake.getTail(); coordinate <= snake.getHead(); coordinate++) {
        int snakeRow = snake.getLocationRow(coordinate);
        int snakeColumn = snake.getLocationColumn(coordinate);
        board[snakeRow][snakeColumn] = 1;
      }

      // Update pellet location on board
      int pelletRow = pellet.getRow();
      int pelletColumn = pellet.getColumn();
      board[pelletRow][pelletColumn] = 3;

      // Print Board to LED Matrix
      // Print first half of board to matrix1
      for (int row = 0; row < 16; row++) {
        for (int column = 0; column < 8; column++) {
          matrix1.drawPixel(column, row, board[row][column]);
        }
      }
      // Print second half of board to matrix2
      for (int row = 0; row < 16; row++) {
        for (int column = 8; column < 16; column++) {
          matrix2.drawPixel(column - 8, row, board[row][column]);
        }
      }
      matrix1.writeDisplay();
      matrix2.writeDisplay();  

      // Print board to Serial: For debugging
      for (int row = 0; row < 16; row++) {
        for (int column = 0; column < 16; column++) {
          Serial.print(board[row][column]);
        }
        Serial.println("");
      }
      Serial.println("");

      // Update snake direction
      // Reset directionChangeAllowed variable so the direction can be updated again for the next frame
      snake.setDirection(snakeFutureDirection);
      directionChangeAllowed = true;

      // Define a boolean variable to check if the snake is out of bounds
      bool outOfBounds = false;
      // Update snake location 
      //Check if snake is in contact with the pellet
      if (snake.getLocationRow(snake.getHead()) == pellet.getRow() && snake.getLocationColumn(snake.getHead()) == pellet.getColumn()) {
        // If the snake is in contact with the pellet, randomize pellet location, increase snake length
        pellet.randomizeLocation();
        outOfBounds = snake.eatPellet();
      } else {
        // Otherwise, just uptdate the snake location 
        outOfBounds = snake.newLocation();
      }
      // Notes: The methods snake.eatPellet() and snake.newLocation() both return bools which specify whether the snake is out of bounds

      // Check if the snake has run into itself
      bool hitItself = false;
      // If the head position is less than the tail position (circular array), iterate from head down to 0 then from max length of the snake location array to tail
      if (snake.getHead() < snake.getTail()) {
        for (int x = snake.getHead() - 2; x >= 0; x--) {
          // Check if the head of the Snake is in the same location as any other part of the snake body 2 locations from its head 
          if (snake.getLocationRow(snake.getHead()) == snake.getLocationRow(x) && snake.getLocationColumn(snake.getHead()) == snake.getLocationColumn(x)) {
            hitItself = true;
            break;
          }
        }
        for (int x = 255; x >= snake.getTail(); x--) {
          // Check if the head of the Snake is in the same location as any other part of the snake body
          if (snake.getLocationRow(snake.getHead()) == snake.getLocationRow(x) && snake.getLocationColumn(snake.getHead()) == snake.getLocationColumn(x)) {
            hitItself = true;
            break;
          }
        }
      } else {
        // If the head position is greater than the tail position 
        for (int x = snake.getHead() - 2; x >= snake.getTail(); x--) {
          // Check if the head of the Snake is in the same location as any other part of the snake body 2 locations from its head
          if (snake.getLocationRow(snake.getHead()) == snake.getLocationRow(x) && snake.getLocationColumn(snake.getHead()) == snake.getLocationColumn(x)) {
            hitItself = true;
            break;
          }
        }
      }
      // If snake hits itself or goes out of bounds, end game
      if (hitItself || outOfBounds) {
        Serial.print("\n");
        Serial.print("GAME OVER!");
        // Sets runGame to false, which causes the code to exit out of the game loop 
        runGame = false;
        // Set gameOverSCreen to true, which will bring up the game over splash screen
        gameOverScreen = true; 
      }
    }

  }

  // Show game over splash screen for 3 seconds
  if(gameOverScreen) { 
    
    matrix1.clear();
    matrix2.clear();
    matrix1.drawBitmap(0, 0, game_over_matrix_1, 8, 16, 1);
    matrix2.drawBitmap(0, 0, game_over_matrix_2, 8, 16, 1);
    matrix1.writeDisplay();
    matrix2.writeDisplay();
    delay(3000); 
    gameOverScreen = false; 
    // Set startScreen to true to bring up the start screen
    startScreen = true;
    //Reset Board
    setUpBoard();
  }

}

//Function to set up a new board
void setUpBoard() {
  // Populate board and boardStatus
  for (int row = 0; row < 16; row++) {
    for (int column = 0; column < 16; column++) {
      // Initialize entire board as off
      board[row][column] = 0;
    }
  }
  // Initialize a snake object
  snake = Snake();

  // Initialize a pellet object
  pellet = Pellet();

}

