#include <Arduino.h>
#define RED 23
#define YELLOW 22
#define GREEN 21
#define button 19

enum state{STOP, WAIT, GO};

state currentState = STOP;

bool isbuttonpressed(){
  if(digitalRead(button) == LOW){
    delay(40);
    if (digitalRead(button) == LOW) return true;
  }
  return false;
}

void halt(unsigned long ms){
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (isbuttonpressed()) {
      while (digitalRead(button) == LOW){
      }
      return;
    }
  }
}

void setup() {
  pinMode(button, INPUT_PULLUP);
  ledcSetup(0,5000,8);
  ledcSetup(1,5000,8);
  ledcSetup(2,5000,8);
  ledcAttachPin(RED,0);
  ledcAttachPin(YELLOW,1);
  ledcAttachPin(GREEN,2);
}

void loop() {
  switch(currentState){
  case STOP:
            ledcWrite(0,255);
            halt(5000);
            ledcWrite(0,0);
            currentState = WAIT;
            break;
  case WAIT:
            ledcWrite(1,255);
            halt(3500);
            ledcWrite(1,0);
            currentState = GO;
            break;
  case GO:
            ledcWrite(2,255);
            halt(3000);
            ledcWrite(2,0);
            currentState = STOP;
            break;
  }
}