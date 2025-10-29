#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

// LCD stuff
constexpr int rs = 11,
en = 12,
d4 = 2,
d5 = 3,
d6 = 4,
d7 = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DHT stuff
#define DHT11_PIN 7
#define DHTTYPE DHT11

DHT DHT11_Sensor(DHT11_PIN, DHTTYPE);

void setup() {
    lcd.begin(16, 2);
    DHT11_Sensor.begin();
    Serial.begin(9600);
}

void loop() {
    delay(1000);

    char buffer[50];
    char tempStr[10];

    double humidity = DHT11_Sensor.readHumidity();
    double temperature = DHT11_Sensor.readTemperature();

    dtostrf(temperature, 4, 1, tempStr); // convert float to string (arduino sprintf cant handle floats :sob:)
    sprintf(buffer, "Temp: %s", tempStr);
    lcd.setCursor(0, 0);
    lcd.print(buffer);

    dtostrf(humidity, 4, 1, tempStr);
    sprintf(buffer, "Humidity: %s", tempStr);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}