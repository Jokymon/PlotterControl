#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include "System.h"

class GCodeParser
{
public:
  GCodeParser();

  void run_command(char* command_buffer);

private:
  void parse(char* command_buffer);
  void execute();

private:
  System system;

  struct CommandState {
    char type;
    int code;
    float x;
    float y;
    float z;
    float f;
    float p;
    float s;
  };

  enum class Positioning { Absolute, Relative };
  Positioning positioning;
  CommandState command_state;
};

#endif
