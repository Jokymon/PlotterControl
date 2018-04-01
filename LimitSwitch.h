#include "enable_if.h"
#include <Arduino.h>

enum class LevelMode {
  Normal,
  Inverted
};

template <uint8_t InputPin, LevelMode PinLevelMode>
class LimitSwitch
{
public:
  LimitSwitch() {
    pinMode(InputPin, INPUT_PULLUP);
  }

  template <typename Dummy = bool>
  auto isHit() const
  -> enable_if_t<PinLevelMode==LevelMode::Normal, Dummy>
  {
    return digitalRead(InputPin) && digitalRead(InputPin);
  }

  template <typename Dummy = bool>
  auto isHit() const
  -> enable_if_t<PinLevelMode==LevelMode::Inverted, Dummy>
  {
    return !digitalRead(InputPin) && !digitalRead(InputPin);
  }
};

