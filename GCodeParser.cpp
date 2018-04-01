#include "GCodeParser.h"

GCodeParser::GCodeParser() :
  axisX{motorX, limitMinX, limitMaxX},
  axisY{motorY, limitMinY, limitMaxY}
{
  motorX.enable();
  motorY.enable();
}

void GCodeParser::run_command(char* command_buffer)
{
  if (command_buffer[0]=='$')
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
    return;
  }
  if (command_buffer[0]=='G' && command_buffer[1]=='2' && command_buffer[2]=='8') {
    axisX.move_to_minimum();
    axisY.move_to_minimum();
  }
}

