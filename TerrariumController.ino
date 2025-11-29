#include <DHT.h>
#include <Servo.h>

#define DHTPIN 2       // whatever digital pin you connected data to
#define DHTTYPE DHT11  // or DHT22 if you have that

int pos = 0;
DHT dht(DHTPIN, DHTTYPE);
Servo myservo;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 test running!");
  myservo.attach(9);
}

void loop() {
  delay(2000);  // DHT11 needs at least 2 sec between reads

  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (t >= 29.5 || h >= 82) {  // safety buffer
    myservo.write(90);         // open the hatch
  }

  if (t <= 26 && h <= 70) {  // everything cooled/dried enough
    myservo.write(0);
  }
  
Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\tTemperature: ");
  Serial.print(t);
  Serial.println("°C");
  delay(2000);
}