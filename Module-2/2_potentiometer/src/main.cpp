#include <Arduino.h>
constexpr int pin = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    int value = analogRead(pin);
    Serial.print("Value:");
    Serial.println(value);
    delay(100);
}