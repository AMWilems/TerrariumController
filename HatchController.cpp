#include "HatchController.h"
#include <Arduino.h>

void HatchController::begin(uint8_t servoPin) {
  myservo.attach(servoPin);
  myservo.write(0);
}

void HatchController::update(float t, float h) {
  unsigned long now = millis();

  if (hatchPos == CLOSED && (t >= 29.5 || h >= 82)) {
    myservo.write(90);
    hatchPos = OPEN;
    hatchMoveStart = now;
    Serial.println("Hatch OPENING");
  } else if (hatchPos == OPEN && (t <= 26 && h <= 70) && (now - hatchMoveStart >= MIN_OPEN_TIME)) {
    myservo.write(0);
    hatchPos = CLOSED;
    hatchMoveStart = now;
    Serial.println("Hatch CLOSING");
  }

  // Hold final position after movement
  if (now - hatchMoveStart > MOVE_TIME) {
    myservo.write(hatchPos == OPEN ? 90 : 0);
  }
}