// Date: 08-mar-2017
// Author: Isidoro Gayo
// Name: Wireless robot control
// Filename: bt_slave_zumo.ino

// -------------------------------------------------------------
// Description: Basic bluetooth controlled robot
// HC-06 is the slave module mounted on zumo bot
// HC-05 is a master module wired on the UNO. A joystick
// is also wired on the arduino.
// 
// Both BT modules must be configured before load the skechtes
// This is the slave sketch to be loaded on zumo robot.
// -------------------------------------------------------------

// Published under the terms of GNU/GPL v3.0 or higher license


#include <SoftwareSerial.h>
#include <ZumoMotors.h>

SoftwareSerial BTSerial(4,5); // RX | TX
ZumoMotors motors;

const int MAXSPEED=300;
const int BAUDRATE=9600;
bool DEBUG=true;

int leftmotor,rightmotor;
byte axis;


void setup(){
  
    Serial.begin(BAUDRATE);
    if (DEBUG){
      Serial.println("Arduino with HC-06 is ready");
    }
    // HC-06 default baud rate is 9600
    BTSerial.begin(BAUDRATE);
    if (DEBUG){
      Serial.println("BTserial started at 9600");
    }
}

void loop(){

  byte recvalue;    // recieved byte value from bt module
  int rawdata;      // data movement without conversion
  
  // Keep reading from HC-06 and 
  // send to Arduino Serial Monitor
  // if debug mode is available
  if (BTSerial.available()){
    
    recvalue=BTSerial.read();

    switch(recvalue){
      case 120:
        axis=120;
        break;
      case 121:
        axis=121;
        break;
      default:
        rawdata=4*recvalue;
        if (DEBUG){
          Serial.print(axis);
          Serial.print('-');
          Serial.println(rawdata);
        }
        botmove(axis,rawdata);
      break;
    }
  }
}


void botmove(byte coord,int val){
  
  // This function determines de bot movement
  // FORWARD, BACKWARD, STOP, TURN RIGHT or LEFT
  
  int bspeedleft,bspeedright=0;
 
  // ---- Turns ----
  if (coord==120){
    // RIGHT
    if (val<505){
      bspeedleft=map(val,504,0,0,MAXSPEED);
      bspeedright=-bspeedleft;
    }
    // LEFT
    if (val>525){
      bspeedleft=map(val,526,1026,0,-MAXSPEED);
      bspeedright=-bspeedleft;
    }
  }
  // ---- Lineal movements ----
  if (coord==121){
    // STOP
    if (val<=525 || val>=505){
      // if the joystick is in central position, the bot 
      // will stay stopped, so call stop motors function
      bspeedleft=0;
      bspeedright=0;
    }
    // BACKWARD
    if (val<505){
      // int val must be mapped first from 
      // 0 for minimum speed(504 int value) to 400 max speed(0)
      // The robot must go backward at 'val' speed
      bspeedleft=map(val,504,0,0,-MAXSPEED);
      bspeedright=bspeedleft;
    }
    // FORWARD
    if (val>525){
      // int val variable must be mapped first within
      // 0 speed(526) to 400 maximun speed (1023-1026)
      // bot must go forward at 'val' speed
      bspeedleft=map(val,526,1026,0,MAXSPEED);
      bspeedright=bspeedleft;
    }
  }

  motors.setLeftSpeed(bspeedleft);
  motors.setRightSpeed(bspeedright);
  delay(2);
}

