#pragma once

#include <Arduino_LED_Matrix.h>

class StatusDisplay {
public:
  void begin();
  void updateStartupAnimation();
  void showWiFiConnected();
  void showSensorError();
  void showCloudStatus(bool ok);
  void update();
  bool isStartupComplete() const { return startupPlayed; }

private:
  ArduinoLEDMatrix matrix;

  bool     startupPlayed = false;
  uint8_t  currentFrame  = 0;
  unsigned long lastFrameTime = 0;
};