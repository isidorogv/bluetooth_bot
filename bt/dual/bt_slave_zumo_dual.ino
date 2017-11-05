// Date: 05-nov-2017
// Author: Isidoro Gayo
// Project: Bluetooth zumo control
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
const int BAUDRATE=9600;
bool DEBUG=false;

int leftmotor,rightmotor;
int lxdata,lydata,rxdata,rydata;
bool lbdata,rbdata;
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
      case 40:      // ( character recieved, packet start...
        nextdata=1;
        break;
      case 41:      // ) char recieved => end of packet...
        // send to Arduino Serial Monitor
        // if debug mode is available
        if (DEBUG){
          Serial.print(lxdata);
          Serial.print(',');
          Serial.print(lydata);
          Serial.print(',');
          Serial.println(lbdata);
          
          Serial.print(rxdata);
          Serial.print(',');
          Serial.print(rydata);
          Serial.print(',');
          Serial.println(rbdata);
        }
        // ... so robot can process recieved command
        botaction(lxdata,lydata,lbdata,rxdata,rydata,rbdata);
        lbdata=false;
        rbdata=false;
        break;
      // Here, we sequentially process the data packet...  
      default:
        // ... x coord. on left joystick...
        if (nextdata==1){
          lxdata=4*recvalue;
          nextdata=2;
          break;
        }
        // ... y coord...
        if (nextdata==2){
          lydata=4*recvalue;
          nextdata=3;
          break;
        }
        // ... and left button...
        if (nextdata==3){
          if (recvalue==0){
            lbdata=true;
          }
          else lbdata=false;
          nextdata=4;
        break;
        }
        // ... and also for right joystick.
        if (nextdata==4){
          rxdata=4*recvalue;
          nextdata=5;
          break;
        }
        if (nextdata==5){
          rydata=4*recvalue;
          nextdata=6;
          break;
        }
        if (nextdata==6){
          if (recvalue==0){
            rbdata=true;
          }
          else rbdata=false;
        }
      break;
    }
  }
}


void botaction(int lxc,int lyc,bool lb,int rxc,int ryc,bool rb){
  
  // This function determines de bot movement

  int lspeed,rspeed;

  // STOP
  if (lyc<=525 && lyc>=505){
    // if the left joystick is in central position, 
    // the bot will stay stopped.
    lspeed=0;
  }
  if (ryc<=525 && ryc>=505){
    rspeed=0;
  }
  
  // Bot Movement depends on speed values for each wheel/track
  // Values higher than 525 will move the wheel/track forward,
  // for values lower than 506, the wheel will move backward.
  // Combining these actions, we'll get turns on left/right.
  
  // On left wheel/track, we get...
  if (lyc<506){
    lspeed=-map(lyc,504,0,0,MAXSPEED);
  }
  if (lyc>525){
    lspeed=map(lyc,526,1026,0,MAXSPEED);
  }
  // ...and for the right wheel/track
  if (ryc<506){
    rspeed=-map(ryc,504,0,0,MAXSPEED);
  }
  if (ryc>525){
    rspeed=map(ryc,526,1026,0,MAXSPEED);
  }
  // x coordinate values are not used this time, but could be 
  // usefull for making movements on a four-wheeled omnibot.
  motors.setLeftSpeed(lspeed);
  motors.setRightSpeed(rspeed);    

  // LED on/off depending on the joystick button.
  // Use this to activate some action in your robot
  // This is only for testing pourposes.
  if (lb){
    digitalWrite(13,HIGH);
  }
  if (rb){
    digitalWrite(13,LOW);
  }  
  delay(2);
}

