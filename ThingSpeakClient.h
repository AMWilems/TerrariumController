#pragma once
bool sendToThingSpeak(float temp, float hum, bool hatchOpen);
bool readTargetValues(float& targetTemp, float& targetHum);