#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include "secrets.h"
#include "HatchController.h"
#include "StatusDisplay.h"
#include "ThingSpeakClient.h"

#define DHTPIN  2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

StatusDisplay   display;
HatchController hatch;

unsigned long lastThingSpeakUpdate = 0;
const unsigned long UPDATE_INTERVAL = 180000UL;  // 3 min

void setup() {
  Serial.begin(115200);

  display.begin();               // ← starts breathing circle immediately
  dht.begin();
  hatch.begin(9);                // servo on pin 9

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  display.showWiFiConnected();
  delay(1500);
}

void loop() {
  display.updateStartupAnimation();
  if (!display.isStartupComplete()) {
    delay(10);
    return;
  }

  // === READ SENSOR (every 2 seconds) ===
  static unsigned long lastRead = 0;
  if (millis() - lastRead >= 2000) {
    lastRead = millis();

    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(hum) || isnan(temp)) {
      Serial.println(F("DHT sensor failed!"));
      display.showSensorError();
      return;
    }

    // Show leaf + hatch status
    display.update(temp, hum, hatch.isOpen());

    // Control the hatch
    hatch.update(temp, hum);

    // === UPLOAD TO THINGSPEAK (every 3 minutes) ===
    static unsigned long lastUpload = 0;
    if (millis() - lastUpload >= 180000UL) {  // 3 minutes
      bool ok = sendToThingSpeak(temp, hum, hatch.isOpen());
      lastUpload = millis();
    }
  }

  delay(10);
}