#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H
#include <Arduino.h>
#include <stdint.h>

enum class Direction : uint8_t {
  Backward=0,
  Forward=1
};

template <uint8_t StepPin, uint8_t DirPin, uint8_t EnablePin>
class StepperMotor
{
public:
  StepperMotor() {
    pinMode(StepPin, OUTPUT);
    pinMode(DirPin, OUTPUT);
    pinMode(EnablePin, OUTPUT);

    disable();
  }

  void enable() {
    digitalWrite(EnablePin, HIGH);
  }

  void disable() {
    digitalWrite(EnablePin, LOW);
  }

  void step(Direction dir) {
    switch (static_cast<uint8_t>(dir) << 2 | digitalRead(StepPin) << 1 | digitalRead(DirPin)) {
      case 0:
      case 5:
        digitalWrite(StepPin, HIGH);
        break;
      case 1:
      case 7:
        digitalWrite(DirPin, LOW);
        break;
      case 2:
      case 4:
        digitalWrite(DirPin, HIGH);
        break;
      case 3:
      case 6:
        digitalWrite(StepPin, LOW);
        break;
    }
    delayMicroseconds(5);
  }
};

#endif
