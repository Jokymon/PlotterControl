#include "GCodeParser.h"

const size_t CommandBufferLength = 128;
size_t command_buffer_position = 0;
char command_buffer[CommandBufferLength];

GCodeParser gcode_parser;

void clear_command_buffer() {
  for (size_t i=0; i<CommandBufferLength; i++)
    command_buffer[i] = '\0';
  command_buffer_position = 0;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");

  clear_command_buffer();
}

void loop() {
  if (Serial.available()==0)
    return;

  char c = Serial.read();

  if ((c=='\n') || (c=='\r')) {
    if (command_buffer_position>0) {
      gcode_parser.run_command(command_buffer);
      Serial.println("ok");
      clear_command_buffer();
    }
    return;
  }

  if (command_buffer_position==CommandBufferLength-1) {
    Serial.println("Max buffer length overrun");
    return;
  }

  command_buffer[command_buffer_position++] = c;
}
