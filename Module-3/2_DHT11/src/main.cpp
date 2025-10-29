#include <Arduino.h>
#include <DHT.h>

#define DHT11_PIN 7
#define DHTTYPE DHT11

DHT DHT11_Sensor(DHT11_PIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    DHT11_Sensor.begin();
}

void loop() {
    delay(1000);
    float humidity = DHT11_Sensor.readHumidity();
    float temperature = DHT11_Sensor.readTemperature();

    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temperature: ");
    Serial.println(temperature);
}