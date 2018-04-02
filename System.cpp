#include "System.h"

System::System() :
  axisX{motorX, limitMinX, limitMaxX},
  axisY{motorY, limitMinY, limitMaxY},
  current_position_units{0.0, 0.0, 0.0}
{
  motorX.enable();
  motorY.enable();
}

void System::home()
{
  axisX.move_to_minimum();
  axisY.move_to_minimum();
  current_position_units.x = 0.0;
  current_position_units.y = 0.0;
  current_position_units.z = 0.0;
}

void System::move_absolute(long x, long y, long z)
{
  PointF delta;
  delta.x = abs(x - current_position_units.x);
  delta.y = abs(y - current_position_units.y);
  delta.z = abs(z - current_position_units.z);

  long max_delta = max(delta.x, delta.y);
  max_delta = max(delta.z, max_delta);

  long x_counter = -max_delta/2;
  long y_counter = -max_delta/2;
  long z_counter = -max_delta/2;

  Direction direction_x = (x >= current_position_units.x) ? Direction::Forward : Direction::Backward;
  Direction direction_y = (y >= current_position_units.y) ? Direction::Forward : Direction::Backward;

  bool can_step_x_now;
  bool can_step_y_now;
  do {
    can_step_x_now = can_step_x(x, direction_x);
    can_step_y_now = can_step_y(y, direction_y);

    if (can_step_x_now) {
      x_counter += delta.x;
      if (x_counter > 0) {
        motorX.step(direction_x);
        x_counter -= max_delta;
        if (direction_x==Direction::Forward)
          current_position_units.x++;
        else
          current_position_units.x--;
      }
    }

    if (can_step_y_now) {
      y_counter += delta.y;
      if (y_counter > 0) {
        motorY.step(direction_y);
        y_counter -= max_delta;
        if (direction_y==Direction::Forward)
          current_position_units.y++;
        else
          current_position_units.y--;
      }
    }

    delay(1);
  } while (can_step_x_now || can_step_y_now);
}

void System::dump_limit_switches()
{
    Serial.print("Limit switches: ");
    if (limitMinX.isHit())
      Serial.print("minX ");
    if (limitMaxX.isHit())
      Serial.print("maxX ");
    if (limitMinY.isHit())
      Serial.print("minY ");
    if (limitMaxY.isHit())
      Serial.print("maxY ");
      
    Serial.println("");
}

bool System::can_step_x(long target_x, Direction direction)
{
  if (target_x==long(current_position_units.x))
    return false;
  else if ((direction==Direction::Forward) && limitMaxX.isHit())
    return false;
  else if ((direction==Direction::Backward) && limitMinX.isHit())
    return false;
  return true;
}

bool System::can_step_y(long target_y, Direction direction)
{
  if (target_y==long(current_position_units.y))
    return false;
  else if ((direction==Direction::Forward) && limitMaxY.isHit())
    return false;
  else if ((direction==Direction::Backward) && limitMinY.isHit())
    return false;
  return true;
}

