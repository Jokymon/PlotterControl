#include "SimpleServo.h"
#include <avr/interrupt.h>
#include <Arduino.h>

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009

const unsigned int SERVO_PERIOD = 20000;

volatile bool isActive = false;
volatile unsigned int high_period;
enum class Phase { Low, High };
Phase currentPhase = Phase::Low;
volatile int servoPin=0;

SIGNAL (TIMER1_COMPA_vect)
{
  if (!isActive)
    return;

  switch (currentPhase)
  {
    case Phase::Low:
      TCNT1 = 0;
      digitalWrite(servoPin, HIGH);
      OCR1A = usToTicks(high_period);
      currentPhase = Phase::High;
      break;
    case Phase::High:
      digitalWrite(servoPin, LOW);
      OCR1A = usToTicks(SERVO_PERIOD);
      currentPhase = Phase::Low;
      break;
  }
}

SimpleServo::SimpleServo() :
  pin(0)
{
}

void SimpleServo::init()
{
  TCCR1A = 0;
  TCCR1B = _BV(CS11);
  TCNT1 = 0;
  TIFR1 |= _BV(OCF1A);
  TIMSK1 |= _BV(OCIE1A);

  high_period = 1500;
}

void SimpleServo::attach(int pin)
{
  this->pin = pin;
  servoPin = pin;
  pinMode(pin, OUTPUT);
  init();
  isActive = true;
}

void SimpleServo::setPosition(float degree)
{
  high_period = map(degree, 0, 180, 1000, 2000);
}

