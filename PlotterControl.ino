#include "LimitSwitch.h"
#include "StepperMotor.h"

LimitSwitch<12, LevelMode::Inverted> limitMinX;
LimitSwitch<13, LevelMode::Inverted> limitMaxX;

LimitSwitch<A2, LevelMode::Inverted> limitMinY;
LimitSwitch<A3, LevelMode::Inverted> limitMaxY;

StepperMotor<10, 11, 4> motor1;
StepperMotor<9, 3, 7> motor2;

void setup() {
  Serial.begin(115200);
  motor1.enable();
  motor2.enable();
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
      motor1.enable();
      for (int i=0; i<100; i++) {
        motor1.step(Direction::Forward);
        delay(1);
      }
      motor1.disable();
      break;
    case 'k':
      motor1.enable();
      for (int i=0; i<100; i++) {
        motor1.step(Direction::Backward);
        delay(1);
      }
      motor1.disable();
      break;
    case 'a':
      motor2.enable();
      for (int i=0; i<100; i++) {
        motor2.step(Direction::Forward);
        delay(1);
      }
      motor2.disable();
      break;
    case 's':
      motor2.enable();
      for (int i=0; i<100; i++) {
        motor2.step(Direction::Backward);
        delay(1);
      }
      motor2.disable();
      break;
  }
}
