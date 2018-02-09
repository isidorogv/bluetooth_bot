// Date: 08-mar-2017
// Author: Isidoro Gayo
// Project: Wireless robot control
// Filename: bt_slave_zumo_enh.ino

// -------------------------------------------------------------
// Description: Enhanced bluetooth controlled robot
// HC-06 is the slave module mounted on zumo bot
// HC-05 is a master module wired on the UNO/NANO. A joystick
// is also wired on this arduino.
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
const BAUDRATE=9600;
bool DEBUG=false;

int leftmotor,rightmotor;
int xdata,ydata;
bool buttondata;
byte nextdata=0;


void setup(){

    // Led pin for testing
    pinMode(13,OUTPUT);
    Serial.begin(BAUDRATE);
    if (DEBUG){
      Serial.println("Arduino with HC-06 is ready");
    }
    // HC-06 default baud rate is 9600
    BTSerial.begin(BAUDRATE);
    if (DEBUG){
      Serial.print("BTserial started at ");
      Serial.println(BAUDRATE);
    }
}

void loop(){

  byte recvalue;    // recieved byte value from bt module
  
  // Keep reading from HC-06 and 
  if (BTSerial.available()){
    
    recvalue=BTSerial.read();

    switch(recvalue){
      case 40:      // ( character has been recieved, packet start
        nextdata=1;
        break;
      case 41:      // ) char recieved => end of packet...
        // send to Arduino Serial Monitor
        // if debug mode is available
        if (DEBUG){
          Serial.print(xdata);
          Serial.print(',');
          Serial.print(ydata);
          Serial.print(',');
          Serial.println(buttondata);
        }
        // ... so robot can process recieved command
        botaction(xdata,ydata,buttondata);
        buttondata=false;
        break;
      default:
        if (nextdata==1){
          xdata=4*recvalue;
          nextdata=2;
          break;
        }
        if (nextdata==2){
          ydata=4*recvalue;
          nextdata=3;
          break;
        }
        if (nextdata==3){
          if (recvalue==0){
            buttondata=true;
          }
          else buttondata=false;
        }
      break;
    }
  }
}


void botaction(int xcoord,int ycoord,bool val){
  
  // This function determines de bot movement

  int speedbase,loffset,roffset;
  bool forward;

  // STOP
  if (ycoord<=525 && ycoord>=505){
      // if the joystick is in central position, 
      // the bot will stay stopped.
      speedbase=0;
      loffset=0;
      roffset=0;
  }
  if (ycoord<506){
    speedbase=map(ycoord,504,0,0,MAXSPEED);
    forward=false;
  }
  if (ycoord>525){
    speedbase=map(ycoord,526,1026,0,MAXSPEED);
    forward=true;
  }
  // Turn right
  if (xcoord<=511){
    roffset=speedbase*map(xcoord,511,0,0,-100)/100;
    loffset=0;
  }
  // Turn left
  if (xcoord>=512){
    loffset=speedbase*map(xcoord,512,1023,0,-100)/100;
    roffset=0;
  }    
  // Forward
  if (forward){
    motors.setLeftSpeed(speedbase+loffset);
    motors.setRightSpeed(speedbase+roffset);    
  }
  // Backward
  else {
    motors.setLeftSpeed(-(speedbase+loffset));
    motors.setRightSpeed(-(speedbase+roffset));
  }
  // LED on/off depending on the joystick button.
  // Use this to activate some action in your robot
  // This is only for testing pourposes.
  if (val){
    digitalWrite(13,HIGH);
  }
  else {
    digitalWrite(13,LOW);
  }  
  delay(2);
}

