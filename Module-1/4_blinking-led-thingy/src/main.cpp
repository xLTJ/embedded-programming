#include <Arduino.h>

int ledPin = 12;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, HIGH);
    Serial.println("ON");
    delay(1000);

    digitalWrite(ledPin, LOW);
    Serial.println("OFF");
    delay(1000);
}