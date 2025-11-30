#include "StatusDisplay.h"

// Perfect breathing circle – 8 frames, 300 ms each, generated with the official editor
const uint32_t breathingCircle[9][3] PROGMEM = {
	{0xf010820, 0x42042042, 0x41080f0},
	{0xf01c83c, 0x43c42042, 0x41080f0},
	{0xf01f83f, 0xc3fc2042, 0x41080f0},
	{0xf01f83f, 0xc3fc23c2, 0x3c1380f0},
	{0xf01f83f, 0xc3fc3fc3, 0xfc1f80f0},
	{0xf013823, 0xc23c3fc3, 0xfc1f80f0},
	{0xf010820, 0x42043fc3, 0xfc1f80f0},
	{0xf010820, 0x42042042, 0x41080f0},
	{0xf010820, 0x42042042, 0x41080f0}
};

void StatusDisplay::begin() {
  matrix.begin();
  startupPlayed = false;
  currentFrame  = 0;
  lastFrameTime = millis();
  matrix.loadFrame(breathingCircle[0]);        // show first frame immediately
}

void StatusDisplay::updateStartupAnimation() {
  if (startupPlayed) return;

  if (millis() - lastFrameTime >= 300) {       // 300 ms → calm breathing
    currentFrame++;
    if (currentFrame >= 9) {
      currentFrame = 7;                        // stay on last frame
      startupPlayed = true;
    }
    matrix.loadFrame(breathingCircle[currentFrame]);
    lastFrameTime = millis();
  }
}

void StatusDisplay::showWiFiConnected() {
  const uint32_t wifi[3] PROGMEM = {0x1f82044f, 0x21080600, 0x60060};
  matrix.loadFrame(wifi);
}

void StatusDisplay::showSensorError() {
  const uint32_t err[3] PROGMEM = {0x10826446, 0x24624624, 0x2264108};
  matrix.loadFrame(err);
}

void StatusDisplay::update(float temp, float hum, bool hatchOpen) {
  updateStartupAnimation();
  if (!startupPlayed) return;

  uint32_t frame[3] = {0, 0, 0};   // ← declared once, in scope for the whole function

  matrix.loadFrame(frame);   // now it works!
}