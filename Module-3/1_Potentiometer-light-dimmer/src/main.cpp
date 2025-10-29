#include <Arduino.h>

constexpr int PotentiometerPin = A0;
constexpr int LightPin = 9; // pwn capable pin

void setup() {
    Serial.begin(9600);
}

void loop() {
    const int potentiometerValue = analogRead(PotentiometerPin); // generates pwn output
    const int brightness = map(potentiometerValue, 0, 1023, 0, 255);

    Serial.println(brightness);
    analogWrite(LightPin, brightness);
}