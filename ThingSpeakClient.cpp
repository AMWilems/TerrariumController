// ThingSpeakClient.cpp
#include "ThingSpeakClient.h"
#include "secrets.h"
#include <Arduino.h>
#include <WiFi.h>

bool sendToThingSpeak(float temp, float hum, bool hatchOpen) {
  // 1. Check WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("[ThingSpeak] No WiFi connection"));
    return false;
  }

  WiFiClient client;

  Serial.print(F("[ThingSpeak] Uploading → Temp="));
  Serial.print(temp, 1);
  Serial.print(F("°C | Hum="));
  Serial.print(hum, 0);
  Serial.print(F("% | Hatch="));
  Serial.print(hatchOpen ? 1 : 0);

  // 2. Connect to ThingSpeak
  if (!client.connect("api.thingspeak.com", 80)) {
    Serial.println(F(" → FAILED (cannot connect)"));
    return false;
  }

  // 3. Build POST data — hatch as 0 or 1
  String postStr = "api_key=" + String(apiKey);
  postStr += "&field1=" + String(temp, 1);
  postStr += "&field2=" + String(hum, 0);
  postStr += "&field3=" + String(hatchOpen ? 1 : 0);  // 0 or 1

  // 4. Send HTTP request
  client.print("POST /update HTTP/1.1\r\n");
  client.print("Host: api.thingspeak.com\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\r\n\r\n");
  client.print(postStr);

  // 5. Wait for response (5 sec timeout)
  unsigned long timeout = millis() + 5000;
  while (!client.available() && millis() < timeout) {
    delay(10);
  }

  if (!client.available()) {
    Serial.println(F(" → FAILED (timeout)"));
    client.stop();
    return false;
  }

  // 6. Read response
  bool success = false;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();
    if (line.startsWith("HTTP/1.1 200 OK")) success = true;
    if (line.toInt() > 0) {
      Serial.print(F(" → SUCCESS (entry #"));
      Serial.print(line.toInt());
      Serial.println(F(")"));
      client.stop();
      return true;
    }
  }

  Serial.println(success ? F(" → SUCCESS") : F(" → FAILED (bad response)"));
  client.stop();
  return success;
}

bool readTargetValues(float& targetTemp, float& targetHum) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("[ThingSpeak] No WiFi for reading targets"));
    return false;
  }

  WiFiClient client;
  if (!client.connect("api.thingspeak.com", 80)) {
    Serial.println(F("[ThingSpeak] Failed to connect for target read"));
    return false;
  }

  // GET the last entry
  client.print(String("GET /channels/") + CHANNEL_ID +
               "/feeds/last?api_key=" + READ_API_KEY + " HTTP/1.1\r\n" +
               "Host: api.thingspeak.com\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis() + 5000;
  while (!client.available() && millis() < timeout) delay(10);

  if (!client.available()) {
    client.stop();
    return false;
  }

  // Parse JSON response (simple way — look for field4 and field5)
  String response = client.readString();
  client.stop();

  int tempPos = response.indexOf("\"field4\":\"");
  int humPos  = response.indexOf("\"field5\":\"");

  if (tempPos != -1 && humPos != -1) {
    tempPos += 10;
    humPos  += 10;
    targetTemp = response.substring(tempPos, response.indexOf('"', tempPos)).toFloat();
    targetHum  = response.substring(humPos,  response.indexOf('"', humPos)).toFloat();

    if (targetTemp >= 15 && targetTemp <= 35 && targetHum >= 50 && targetHum <= 95) {
      return true;
    }
  }
  return false;
}