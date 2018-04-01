#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include "LimitSwitch.h"
#include "StepperMotor.h"
#include "Axis.h"

typedef LimitSwitch<A2, LevelMode::Inverted> LimitMinX;
typedef LimitSwitch<A3, LevelMode::Inverted> LimitMaxX;
typedef LimitSwitch<12, LevelMode::Inverted> LimitMinY;
typedef LimitSwitch<13, LevelMode::Inverted> LimitMaxY;

typedef StepperMotor<9, 3, 7> MotorX;
typedef StepperMotor<10, 11, 4> MotorY;

class GCodeParser
{
public:
  GCodeParser();

  void run_command(char* command_buffer);

private:
  LimitMinX limitMinX;
  LimitMaxX limitMaxX;
  LimitMinY limitMinY;
  LimitMaxY limitMaxY;

  MotorX motorX;
  MotorY motorY;

  Axis<MotorX, LimitMinX, LimitMaxX> axisX;
  Axis<MotorY, LimitMinY, LimitMaxY> axisY;
};

#endif
