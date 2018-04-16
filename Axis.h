#ifndef AXIS_H
#define AXIS_H

#include "StepperMotor.h"

template <typename Motor, typename LimitMin, typename LimitMax>
class Axis
{
public:
  Axis(Motor &motor, LimitMin &limitMin, LimitMax &limitMax) :
    motor(motor),
    limitMin(limitMin),
    limitMax(limitMax)
  { }

  void move_to_minimum()
  {
    while (!limitMin.isHit() && !limitMax.isHit()) {
      motor.step(Direction::Backward);
      delay(1);
    }

    while (limitMin.isHit() || limitMax.isHit()) {
      motor.step(Direction::Forward);
      delay(100);
    }
  }

private:
  Motor &motor;
  LimitMin &limitMin;
  LimitMax &limitMax;
};

#endif
