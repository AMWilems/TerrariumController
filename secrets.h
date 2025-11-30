#ifndef SECRETS_H
#define SECRETS_H

// WiFi credentials (replace with yours)
constexpr const char* ssid     = "animalHAUS";      // Your SSID
constexpr const char* password = "L00kAtAllTh0seCh1k3ns!";          // Your WiFi password

// ThingSpeak Write API Key (from your channel's API Keys tab)
constexpr const char* apiKey   = "ERS50ZO8EHAH84PH";            // 16-character key

enum class HatchPosition { CLOSED, OPEN };
#endif // SECRETS_H