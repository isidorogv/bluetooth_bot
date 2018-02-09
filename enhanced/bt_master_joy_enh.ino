// Date: 08-mar-2017
// Author: Isidoro Gayo
// Name: Wireless robot control
// Filename: bt_master_joy_enh.ino

// Description: Enhanced version of bluetooth controlled 
// zumo robot. In this version, the robot can recieve both
// x and y joystick coordinates in order to perform a smooth
// turn at the same time it moves forward/backward.
// ------------------------------------------------------------
// This file MUST BE loaded into the master arduino device.
// Master and Slave BT modules must be configured before load 
// the skechtes. This is the master sketch.
// ------------------------------------------------------------
// This example code is released under the terms of
// GNU/GPL v3.0 or higher for the public domain.

//Include the LCD Library.
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Declare the LCD object and define pins.
// Itead LCD keyshield used for debugging
LiquidCrystal lcd(8,9,4,5,6,7);
SoftwareSerial BTSerial(A4,A5); // RX | TX

void setup() {

  // Button joystick is input
  pinMode(A3,INPUT);
  // HC-05 master default baud rate is 9600
  BTSerial.begin(9600);
  //Start the LCD object.
  lcd.begin(16,2);
  //Start with a clear LCD screen.
  lcd.clear();
  
}

void loop() {

  int sentval01=analogRead(1)/4;
  int sentval02=analogRead(2)/4;
  byte sentval03=digitalRead(A3);
  
  // ---- For debugging pourposes only ----
  lcd.setCursor(0,0);
  lcd.print(analogRead(1));
  lcd.print(" ");
  lcd.print(sentval01);
  lcd.print("    ");
  
  lcd.setCursor(0,1);
  lcd.print(analogRead(2));
  lcd.print(" ");
  lcd.print(sentval02);
  lcd.print("   ");
  lcd.print(sentval03);
  lcd.print("   ");
  // --------------------------------------
  // Now, the x and y joystick coordinates are sent sequentialy
  // and procesed at the same time in the slave code
  BTSerial.write('(');
  BTSerial.write(sentval01);
  BTSerial.write(sentval02);
  BTSerial.write(sentval03);
  BTSerial.write(')');
}

