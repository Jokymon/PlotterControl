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
  parse(command_buffer);
  execute();
}

void GCodeParser::parse(char* command_buffer)
{
  char reg = ' ';
  char* parse_pos = command_buffer;

  while (*parse_pos != '\0') {
    if (isAlpha(*parse_pos)) {
      reg = *parse_pos;
      if (reg=='G' || reg=='M') {
        command_state.type = reg;
      }
      ++parse_pos;
    }
    else if (isDigit(*parse_pos)) {
      double value = strtod(parse_pos, &parse_pos);
      switch (reg) {
        case 'G':
        case 'M':
          command_state.code = int(value);
          break;
        case 'F':
          command_state.f = value;
          break;
        case 'P':
          command_state.p = value;
          break;
        case 'S':
          command_state.s = value;
          break;
        case 'X':
          command_state.x = value;
          break;
        case 'Y':
          command_state.y = value;
          break;
        case 'Z':
          command_state.z = value;
          break;
      }
      reg = ' ';
    }
    else if (isSpace(*parse_pos)) {
      // just skip
      ++parse_pos;
    }
    else if (*parse_pos == '$') {
      command_state.type = *parse_pos;
      ++parse_pos;
    }
  }
}

void GCodeParser::execute()
{
  switch (command_state.type) {
    case '$':
      dump_limit_switches();
      break;
    case 'G':
      switch (command_state.code) {
        case 28:
          axisX.move_to_minimum();
          axisY.move_to_minimum();
          break;
      }
      break;
  }
}

void GCodeParser::dump_limit_switches()
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

