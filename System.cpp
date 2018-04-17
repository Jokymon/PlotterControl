#include "System.h"

System::System() :
  axisX{motorX, limitMinX, limitMaxX},
  axisY{motorY, limitMinY, limitMaxY},
  current_position{0.0, 0.0, 90.0}
{
  motorX.enable();
  motorY.enable();
}

void System::init()
{
  servoZ.attach(A1);
}

void System::home()
{
  axisX.move_to_minimum();
  axisY.move_to_minimum();

  current_position.x = 0.0;
  current_position.y = 0.0;

  current_position.z = 90.0;
  servoZ.setPosition(current_position_units.z);
}

void System::move_absolute(long x, long y, long z)
{
  if (z<0)
    z = 0;
  if (z>180)
    z = 180;

  PointF delta;
  delta.x = abs(x - current_position.x);
  delta.y = abs(y - current_position.y);

  long max_delta = max(delta.x, delta.y);

  long x_counter = -max_delta/2;
  long y_counter = -max_delta/2;

  Direction direction_x = (x >= current_position.x) ? Direction::Forward : Direction::Backward;
  Direction direction_y = (y >= current_position.y) ? Direction::Forward : Direction::Backward;

  servoZ.setPosition(z);
  current_position.z = z;

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
          current_position.x++;
        else
          current_position.x--;
      }
    }

    if (can_step_y_now) {
      y_counter += delta.y;
      if (y_counter > 0) {
        motorY.step(direction_y);
        y_counter -= max_delta;
        if (direction_y==Direction::Forward)
          current_position.y++;
        else
          current_position.y--;
      }
    }

    delay(1);
  } while (can_step_x_now || can_step_y_now);
}

void  System::arc_around_relative_to(float c_dx, float c_dy, float to_x, float to_y, Orientation orientation)
{
  PointF center;
  center.x = current_position.x + c_dx;
  center.y = current_position.y + c_dy;

  float a_x = -c_dx;
  float a_y = -c_dy;
  float b_x = to_x - center.x;
  float b_y = to_y - center.y;

  float angleA, angleB;
  if (orientation==Clockwise) {
    angleA = atan2(b_y, b_x);
    angleB = atan2(a_y, a_x);
  } else {
    angleA = atan2(a_y, a_x);
    angleB = atan2(b_y, b_x);
  }

  if (angleB <= angleA) angleB += 2 * M_PI;
  float angle = angleB - angleA;

  float radius = sqrt(a_x * a_x + a_y * a_y);
  float length = radius * angle;

  int steps = (int)ceil(length / 0.5); // TODO curve_section
  PointF p;
  for (int s=1; s <= steps; s++) {
    int step = (orientation==CounterClockwise) ? s : steps-s;
    p.x = center.x + radius * cos(angleA + angle * ((float)step / steps));
    p.y = center.y + radius * sin(angleA + angle * ((float)step / steps));
    move_absolute(p.x, p.y, current_position.z);
  }
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

void System::set_position(float x, float y, float z)
{
  current_position.x = x;
  current_position.y = y;
  current_position.z = z;
}

float System::get_x() const
{
  return current_position.x;
}

float System::get_y() const
{
  return current_position.y;
}

float System::get_z() const
{
  return current_position.z;
}

bool System::can_step_x(long target_x, Direction direction)
{
  if (target_x==long(current_position.x))
    return false;
  else if ((direction==Direction::Forward) && limitMaxX.isHit())
    return false;
  else if ((direction==Direction::Backward) && limitMinX.isHit())
    return false;
  return true;
}

bool System::can_step_y(long target_y, Direction direction)
{
  if (target_y==long(current_position.y))
    return false;
  else if ((direction==Direction::Forward) && limitMaxY.isHit())
    return false;
  else if ((direction==Direction::Backward) && limitMinY.isHit())
    return false;
  return true;
}

