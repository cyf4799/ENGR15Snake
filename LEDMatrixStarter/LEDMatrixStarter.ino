/*************************************************** 
  This is a library for our I2C LED Backpacks & FeatherWings

  Designed specifically to work with the Adafruit 16x8 LED Matrix FeatherWing

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x16minimatrix matrix1 = Adafruit_8x16minimatrix();
Adafruit_8x16minimatrix matrix2 = Adafruit_8x16minimatrix();

void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("16x8 LED Mini Matrix Test");
  
  matrix1.begin(0x70);  // pass in the address
  matrix2.begin(0x72);
}

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };

void loop() {
  matrix1.drawPixel(0,0,1);
  //top right is 0 
  //Row: 0 - 7
  matrix2.drawPixel(0, 0, 1);
  matrix1.writeDisplay();
  matrix2.writeDisplay();
}
