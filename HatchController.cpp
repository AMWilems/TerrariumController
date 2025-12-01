#include "HatchController.h"
#include <Arduino.h>

constexpr float TEMP_HYSTERESIS = 2.5;   // °C
constexpr float HUM_HYSTERESIS  = 6.0;   // % RH

void HatchController::begin(uint8_t servoPin) {
  myservo.attach(servoPin);
  myservo.write(0);
  Serial.println("HATCH CLOSED");
}

void HatchController::update(float t, float h, float targetTemp, float targetHum) {
  unsigned long now = millis();
  /*Serial.print("Current params: ");
  Serial.print(targetTemp, 1);
  Serial.print(F("°C  Hum: "));
  Serial.print(targetHum, 0);
  Serial.println(F("%"));*/
  if (hatchPos == CLOSED && (t >= targetTemp + TEMP_HYSTERESIS || h >= targetHum + HUM_HYSTERESIS)) {
    myservo.write(90);
    hatchPos = OPEN;
    hatchMoveStart = now;
    Serial.println("Hatch OPENING");
  } else if (hatchPos == OPEN && (t <= targetTemp && h <= targetHum) && (now - hatchMoveStart >= MIN_OPEN_TIME)) {
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