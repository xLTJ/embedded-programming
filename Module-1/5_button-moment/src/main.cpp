#include <Arduino.h>

constexpr  int ledPin = 12;
constexpr  int buttonPin = 2;
bool lampOn = false;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
}

void loop() {
    // if button is pressed, do shit
    if (digitalRead(buttonPin) == LOW) {
        lampOn = !lampOn; // update state
        digitalWrite(ledPin, lampOn); // update lamp

        Serial.println(lampOn ? "Lamp: ON" : "Lamp: OFF");
        delay(500); // debounce
    }
}