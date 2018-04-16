#include "GCodeParser.h"

const PointF StepsPerMM = {21.8, 22.85, 1.0};
const PointF StepsPerInch = {48.0, 48.0, 1.0};

GCodeParser::GCodeParser() :
  positioning(Positioning::Absolute),
  steps_per_unit(StepsPerMM)
{
}

void GCodeParser::init()
{
  system.init();
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
        case 'I':
          command_state.i = value;
          break;
        case 'J':
          command_state.j = value;
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
      system.dump_limit_switches();
      break;
    case 'G':
      switch (command_state.code) {
        case 0:
        case 1:
          system.move_absolute(command_state.x * steps_per_unit.x,
                               command_state.y * steps_per_unit.y,
                               command_state.z * steps_per_unit.z);
          break;
        case 2:
          system.arc_around_relative_to(command_state.i * steps_per_unit.x,
                                        command_state.j * steps_per_unit.y,
                                        command_state.x * steps_per_unit.x,
                                        command_state.y * steps_per_unit.y,
                                        System::Clockwise);
          break;
        case 3:
          system.arc_around_relative_to(command_state.i * steps_per_unit.x,
                                        command_state.j * steps_per_unit.y,
                                        command_state.x * steps_per_unit.x,
                                        command_state.y * steps_per_unit.y,
                                        System::CounterClockwise);
          break;
        case 20:
          steps_per_unit = StepsPerInch;
          break;
        case 21:
          steps_per_unit = StepsPerMM;
          break;
        case 28:
          system.home();
          break;
        case 90:
          positioning = Positioning::Absolute;
          break;
        case 91:
          positioning = Positioning::Relative;
          break;
      }
      break;
  }
}

