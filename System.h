#ifndef SYSTEM_H
#define SYSTEM_H

#include "LimitSwitch.h"
#include "StepperMotor.h"
#include "SimpleServo.h"
#include "Axis.h"

struct PointF {
  float x;
  float y;
  float z;
};

typedef LimitSwitch<A2, LevelMode::Inverted> LimitMinX;
typedef LimitSwitch<A3, LevelMode::Inverted> LimitMaxX;
typedef LimitSwitch<8, LevelMode::Inverted> LimitMinY;
typedef LimitSwitch<2, LevelMode::Inverted> LimitMaxY;

typedef StepperMotor<9, 3, 7> MotorX;
typedef StepperMotor<10, 11, 4> MotorY;

class System
{
public:
  System();

  void init();
  void home();
  void move_absolute(long x, long y, long z);
  void dump_limit_switches();

private:
  bool can_step_x(long target_x, Direction direction);
  bool can_step_y(long target_y, Direction direction);

private:
  LimitMinX limitMinX;
  LimitMaxX limitMaxX;
  LimitMinY limitMinY;
  LimitMaxY limitMaxY;

  MotorX motorX;
  MotorY motorY;
  SimpleServo servoZ;

  Axis<MotorX, LimitMinX, LimitMaxX> axisX;
  Axis<MotorY, LimitMinY, LimitMaxY> axisY;

  PointF current_position_units;
};

#endif
