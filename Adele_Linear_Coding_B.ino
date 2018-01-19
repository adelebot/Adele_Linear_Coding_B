#include <Servo.h>
#include <Adafruit_CircuitPlayground.h>
#include "Adele_Robot.h"
#include "PharmacodeRead.h"

#define  PIN   6
#define  NBARS 4

pharmacodeRead pharmacode(NBARS, PIN);//creaes reader for NBARS


void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  Serial.begin(9600);
  pixelsOff();
  adeleBegin();
  stopServos();
}

void loop() {
  mode1();
}



/**************** modes and functions **********/

void mode1() {
  setPixelRules();

  //if switch to RIGHT then read codes
  if (CircuitPlayground.slideSwitch()) {

    //read code when left button mainteined pressed

    if (CircuitPlayground.leftButton() && !pharmacode.stopState) {
      if (pharmacode.currentRes < 10) { //no more than 10 rules
        pharmacode.readCode();
      }
    }

    if (pharmacode.stopState) {//entire code was read
      CircuitPlayground.playTone(440, 100);
      pharmacode.resetCode();
    }

    //erase one rule with right button
    if (CircuitPlayground.rightButton()) {
      pharmacode.eraseLast();
      pharmacode.resetCode();
      CircuitPlayground.playTone(880, 100);
      delay(200);//to avoid bouncing
    }
    // when siwtch LEFT it executes the rules
  } else {

    //waits 1 sec before start
    delay(1000);
    executeCode();
    pharmacode.resetCode();
    pharmacode.resetLog();
    stopServos();
    pixelsOff();
  }
}



//Code execution and removes rules after applied
void executeCode() {
  for (int i = 0; i < pharmacode.currentRes; i++) {
    switch (pharmacode.resultLog[i]) {
      case 16://move forward
        forward();
        delay(1000);
        break;
      case 17://move backward
        backward();
        delay(1000);
        break;
      case 18://left turn
        left();
        delay(600);
        break;
      case 19://right turn
        right();
        delay(600);
        break;
      case 20://openClaw
        openGarra();
        delay(300);
        break;
      case 21://closeClaw
        closeGarra();
        delay(300);
        break;
      case 22://play tune
        CircuitPlayground.playTone(440, 250);
        CircuitPlayground.playTone(349, 125);
        CircuitPlayground.playTone(523, 250);
        break;
      case 23://color wheel
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(25 * i));
          delay(150);
        }
        break;
    }
    stopServos();
    pharmacode.resultLog[i] = 0;
    setPixelRules();
  }
}


//turns off all pixels (to avoid using clerPixles)
void pixelsOff() {
  for (int i = 0; i < 10; i++)
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
}

//set pixel colors depending on code
void setPixelRules() {
  for (int i = 0; i < 10; i++) {
    Serial.println(pharmacode.currentRes);
    switch (pharmacode.resultLog[i]) {
      case 0:
        CircuitPlayground.setPixelColor(i, 0, 0, 0);
        break;
      case 16:
        CircuitPlayground.setPixelColor(i, 0, 255, 0);
        break;
      case 17:
        CircuitPlayground.setPixelColor(i, 255, 0, 0);
        break;
      case 18:
        CircuitPlayground.setPixelColor(i, 0, 0, 255);
        break;
      case 19:
        CircuitPlayground.setPixelColor(i, 255, 255, 0);
        break;
      case 20:
        CircuitPlayground.setPixelColor(i, 255, 0, 255);
        break;
      case 21:
        CircuitPlayground.setPixelColor(i, 56, 149, 255);
        break;
      case 22:
        CircuitPlayground.setPixelColor(i, 244, 143, 66);
        break;
      case 23:
        CircuitPlayground.setPixelColor(i, 255, 102, 178);
        break;
      default:
        CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }
  }
}




