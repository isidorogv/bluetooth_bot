// Date: 05-nov-2017
// Author: Isidoro Gayo
// Name: Wireless robot control
// Filename: bt_master_dual_joy.ino

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
#include <LiquidCrystal_I2C.h>  // library by Francisco Malpartida
#include <SoftwareSerial.h>
#include <Wire.h>


// Declare the LCD object and define pins.
// PCF8574-based I2C adaptor for LCD (used for debugging)
LiquidCrystal_I2C lcd(0x26,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial BTSerial(2,3); // digital pins for RX | TX

const int BAUDRATE=9600;
bool DEBUG=false;

byte lxpin=0;             // potentiometer pin on left x axis...
byte rxpin=1;             // ... left y axis...
byte lypin=2;             // ... right x axis...
byte rypin=3;             // ... and right y axis.
byte lbjoy=4;             // input for left joystick button...
byte rbjoy=5;             // ... and right joystick button.



void setup() {

  // joystick's buttons are input
  pinMode(lbjoy,INPUT);
  pinMode(rbjoy,INPUT);
  
  // HC-05 master default bluetooth baud rate is 9600
  BTSerial.begin(BAUDRATE);
  
  if(DEBUG){
    // Start serial com for debugging...
    Serial.begin(BAUDRATE);
    Serial.println("Arduino with HC-05 is ready");
    Serial.print("Serial debug started at ");
    Serial.println(BAUDRATE);
    // Init the LCD object.
    lcd.begin(16,2);
    // Start with a clear LCD screen.
    lcd.clear();
    // ...and turn on the backlight
    lcd.backlight();
    lcd.home();
  }
}

void loop() {

  byte lx=analogRead(lxpin)/4;
  byte ly=analogRead(rxpin)/4;
  byte rx=analogRead(lypin)/4;
  byte ry=analogRead(rypin)/4;
 
  // ---- For debugging pourposes only ----
  if(DEBUG){
    lcd.setCursor(0,0);
    lcd.print(analogRead(lxpin));
    lcd.print(" ");
    lcd.print(lx);
    lcd.print(" ");
    lcd.print(analogRead(rxpin));
    lcd.print(" ");
    lcd.print(ly);
    lcd.print("    ");
    
    lcd.setCursor(0,1);
    lcd.print(analogRead(lypin));
    lcd.print(" ");
    lcd.print(rx);
    lcd.print(" ");
    lcd.print(analogRead(rypin));
    lcd.print(" ");
    lcd.print(ry);
    lcd.print("    ");

    Serial.print(digitalRead(lbjoy));
    Serial.print(',');
    Serial.println(digitalRead(rbjoy));
  }
  // --------------------------------------
  // Now, the x and y joystick coordinates are sent sequentialy
  // and procesed at the same time in the slave code. First,
  // we send left joystick data, and next, right joystick.
  BTSerial.write('(');
  BTSerial.write(lx);
  BTSerial.write(ly);
  BTSerial.write(digitalRead(lbjoy));
  BTSerial.write(rx);
  BTSerial.write(ry);
  BTSerial.write(digitalRead(rbjoy));
  BTSerial.write(')');
}

