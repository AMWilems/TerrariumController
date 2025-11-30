#pragma once
#include <Servo.h>

class HatchController {
public:
  void begin(uint8_t servoPin);
  void update(float temp, float humidity);
  // called every loop
  bool isOpen() const { return hatchPos == OPEN; }

private:
  enum HatchPosition { CLOSED, OPEN };
  HatchPosition hatchPos = CLOSED;
  Servo myservo;
  unsigned long hatchMoveStart = 0;

  const unsigned long MOVE_TIME     = 1000;     // ms to complete travel
  const unsigned long MIN_OPEN_TIME = 600000UL; // 10 min
};