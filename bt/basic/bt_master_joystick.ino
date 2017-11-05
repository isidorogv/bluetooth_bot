// Date: 08-mar-2017
// Author: Isidoro Gayo
// Project: Bluetooth zumo control
// Filename: bt_master_joystick.ino

// -------------------------------------------------------------
// Description: Enhanced bluetooth controlled robot
// HC-06 is the slave module mounted on zumo bot
// HC-05 is a master module wired on the UNO/NANO. A joystick
// is also wired on this arduino.
// 
// Both BT modules must be configured before load the skechtes
// This is the master sketch to be loaded on arduino board.
// -------------------------------------------------------------

// Published under the terms of GNU/GPL v3.0 or higher license

//Include the LCD Library and emulated serial port.
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//Declare the LCD object and define pins.
// Itead LCD keyshield
LiquidCrystal lcd(8,9,4,5,6,7);
SoftwareSerial BTSerial(A4,A5); // RX | TX

const int BAUDRATE=9600;

void setup() {
  
  // HC-05 master default baud rate is 9600
  BTSerial.begin(BAUDRATE);
  //Start the LCD object.
  lcd.begin(16,2);
  //Start with a clear LCD screen.
  lcd.clear();
  
}

void loop() {

  int sentval01=analogRead(1)/4;
  int sentval02=analogRead(2)/4;
  
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
  lcd.print("    ");
  // --------------------------------------
  
  // Turns will have preference over forward/barckward movements,
  // so we can perform just one kind of movement at once
  if (analogRead(1)<505 || analogRead(1)>525){
    BTSerial.write('x');
    BTSerial.write(sentval01);
  }
  else{ 
    BTSerial.write('y');
    BTSerial.write(sentval02);
  }
}

