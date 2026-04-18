/**
 * GemStylus (gemstylus.cpp)
 * Derrick Sobodash <derrick@sobodash.com>
 * Version 1.1.0
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 */

// Set this to zero and compile the sketch. Position the stylus at zero degrees
// relative to the faceting lap and record the value here.
float offset = 1.7;


#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <MPU6050.h>

// Initialize the 7-segment display and accelerometer
Adafruit_7segment matrix = Adafruit_7segment();
MPU6050 mpu;
Vector  normAccel;

// Used throughout the loops
int16_t angle = 0;
int16_t last_angle = 0;
uint8_t pollcount  = 70;

// 0-9
const byte num[10] = {
  B00111111, B00000110, B01011011, B01001111, B01100110, // 01234
  B01101101, B01111101, B00000111, B01111111, B01101111  // 56789
};

// A-Z
const byte letter[27] = {
  B01110111, B01111100, B01011000, B01011110, B01111001, // ABCDE
  B01110001, B01101111, B01110100, B00110000, B00001110, // FGHIJ
  B11110110, B00111000, B11010100, B01010100, B01011100, // KLMNO
  B01110011, B01100111, B01010000, B01101101, B01111000, // PQRST
  B00011100, B10011100, B10011100, B01110110, B01101110, // UVWXY
  B01011011, B00000000                                   // Z
};

void setup() {
  Wire.begin();

  matrix.begin(0x70);
  matrix.writeDigitRaw(0, letter[1]);  // b
  matrix.writeDigitRaw(1, letter[14]); // o
  matrix.drawColon(false);
  matrix.writeDigitRaw(3, letter[14]); // o
  matrix.writeDigitRaw(4, letter[19]); // t
  matrix.writeDisplay();

  // Stall until the MPU sensor picks up, grab 255 gyro readings,
  // and wait for it to stabilize
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){}
  mpu.calibrateGyro(255);
  delay(100);

  Serial.begin(115200);
}

void loop() {
  double ax = 0, ay = 0, az = 0;

  // Collect pollcount sensor readings and average each axis
  for(int8_t i = 0; i < pollcount; i++) {
    normAccel = mpu.readNormalizeAccel();
    ax+=normAccel.XAxis;
    ay+=normAccel.YAxis;
    az+=normAccel.ZAxis;
  }
  ax = ax / pollcount;
  ay = ay / pollcount;
  az = az / pollcount;

  // Calculate the pitch to find our angle.
  // We cast it to a double while multiplying by 10 to limit our result
  // to one decimal
  angle = ((-(atan2(ax, sqrt(ay * ay + az * az)) * 180.0) / M_PI) + 90) * 10;

  // Calculate the roll to detect when the stylus moves past zero
  int16_t roll = (atan2(ay, az) * 180.0) / M_PI;
  if(roll < 0) {
    angle = angle * -1;
  }

  // Add our known offset from zero
  angle += (offset * 10);

  updateDigits();
}


// Split up the digits and draw them to the LED display
void updateDigits() {
  // Cast the angle to absolute (removes negative for easy math).
  uint16_t temp = abs(angle);
  
  // Write the last two digits, which require no processing
  matrix.writeDigitRaw(4, num[temp % 10]);
  matrix.writeDigitRaw(3, num[(temp / 10 % 10)] | 0x80); // decimal point

  // Dirty code to blank the upper digits if insignificant,
  // write the number, or write a negative symbol.
  if((temp / 100 % 10) > 0)
    matrix.writeDigitRaw(1, num[(temp / 100 % 10)]);
  else if((temp / 100 % 10) == 0 && temp > 100)
    matrix.writeDigitRaw(1, num[0]);
  else if(angle < 0)
    matrix.writeDigitRaw(1, B01000000);
  else
    matrix.writeDigitRaw(1, 0);
  if((temp / 1000 % 10) > 0)
    matrix.writeDigitRaw(0, num[(angle / 1000 % 10)]);
  else if(angle < 0 && (temp / 100 % 10) > 0)
    matrix.writeDigitRaw(0, B01000000);
  else
    matrix.writeDigitRaw(0, 0);

  // Update the display
  matrix.writeDisplay();
}
