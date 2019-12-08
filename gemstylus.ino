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

#include <Wire.h>
// MPU6050 lib from https://github.com/jrowberg/i2cdevlib/
#include "I2Cdev.h"
#include "MPU6050.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
MPU6050 mpu;

// Used throughout the loops
int16_t angle = 0;
int16_t last_angle = 0;
unsigned int pollcount  = 500;
unsigned int lifesigns = 0;

int16_t axr, ayr, azr;

void setup() {
  Wire.begin();
  mpu.initialize();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print(mpu.testConnection() ? "Connected to faceter" : "Faceter conn failed");

  // Update internal sensor offsets for my specific MPU6050
  // Yours will vary
  mpu.setXAccelOffset(-2726);
  mpu.setYAccelOffset(-2388);
  mpu.setZAccelOffset(1074);
  mpu.setXGyroOffset(65);
  mpu.setYGyroOffset(147);
  mpu.setZGyroOffset(37);

  // Digital low-pass filter mode
  mpu.setDLPFMode(6); // 5Hz
  // Accelerometer sensitivity
  mpu.setFullScaleAccelRange(0);  
}

void loop() {
  long ax = 0, ay = 0, az = 0;

  // Collect pollcount sensor readings and average each axis
  for(uint16_t i = 0; i < pollcount; i++) {
    mpu.getAcceleration(&axr, &ayr, &azr);
    ax+=axr;
    ay+=ayr;
    az+=azr;
  }

  ax = ax / pollcount;
  ay = ay / pollcount;
  az = az / pollcount;

  // Calculate the pitch to find our angle.
  // We cast it to a double while multiplying by 10 to limit our result
  // to one decimal
  angle = ((-(atan2(ay, sqrt(ax * ax + az * az)) * 180.0) / M_PI) + 90.0) * 100.0;

  // Calculate the roll to detect when the stylus moves past zero
  int16_t roll = (atan2(ay, az) * 180.0) / M_PI;
  if(roll < 0) {
    angle = angle * -1;
  
  }

  // Cast the angle to absolute (removes negative for easy math).
  double absangle = abs(angle) / 100.0;

  // Sign of life counter for troubleshooting sensor vs. mainboard issues
  lifesigns++;

  // Write to the display
  lcd.setCursor(0, 0);
  lcd.print(String(absangle,2));
  lcd.print("          ");
  lcd.setCursor(0,3);
  lcd.print(lifesigns);
}
