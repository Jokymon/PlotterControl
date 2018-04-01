#include "LimitSwitch.h"
#include "StepperMotor.h"

LimitSwitch<A2, LevelMode::Inverted> limitMinX;
LimitSwitch<A3, LevelMode::Inverted> limitMaxX;

LimitSwitch<12, LevelMode::Inverted> limitMinY;
LimitSwitch<13, LevelMode::Inverted> limitMaxY;

StepperMotor<9, 3, 7> motorX;
StepperMotor<10, 11, 4> motorY;

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
  motorX.enable();
  motorY.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  char c = Serial.read();
  switch (c) {
    case 'q':
      if (limitMinX.isHit())
        Serial.println("MinX is hit!");
      else
        Serial.println("MinX is not hit");
      break;
    case 'w':
      if (limitMaxX.isHit())
        Serial.println("MaxX is hit!");
      else
        Serial.println("MaxX is not hit");
      break;
    case 'e':
      if (limitMinY.isHit())
        Serial.println("MinY is hit!");
      else
        Serial.println("MinY is not hit");
      break;
    case 'r':
      if (limitMaxY.isHit())
        Serial.println("MaxY is hit!");
      else
        Serial.println("MaxY is not hit");
      break;
    case 'j':
      motorX.enable();
      for (int i=0; i<100; i++) {
        motorX.step(Direction::Backward);
        delay(1);
      }
      motorX.disable();
      break;
    case 'k':
      motorX.enable();
      for (int i=0; i<100; i++) {
        motorX.step(Direction::Forward);
        delay(1);
      }
      motorX.disable();
      break;
    case 'a':
      motorY.enable();
      for (int i=0; i<100; i++) {
        motorY.step(Direction::Backward);
        delay(1);
      }
      motorY.disable();
      break;
    case 's':
      motorY.enable();
      for (int i=0; i<100; i++) {
        motorY.step(Direction::Forward);
        delay(1);
      }
      motorY.disable();
      break;
  }
}
