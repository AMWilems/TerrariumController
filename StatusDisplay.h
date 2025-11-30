#pragma once

// THIS IS THE CORRECT INCLUDE FOR UNO R4 WiFi
#include <Arduino_LED_Matrix.h>     // <-- this pulls in everything we need

class StatusDisplay {
public:
  void begin();
  void updateStartupAnimation();     // call every loop
  void showWiFiConnected();
  void showSensorError();
  void showCloudStatus(bool ok);
  void update(float temp, float hum, bool hatchOpen);
  bool isStartupComplete() const { return startupPlayed; }

private:
  ArduinoLEDMatrix matrix;           // <-- now known because of the include above

  bool     startupPlayed = false;
  uint8_t  currentFrame  = 0;
  unsigned long lastFrameTime = 0;
};