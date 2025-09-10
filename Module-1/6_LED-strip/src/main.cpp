#include <Arduino.h>

constexpr int ledPins[] = {12, 11, 10, 9};
constexpr  int buttonPin = 2;
constexpr long interval = 500;

// other variables
constexpr int ledAmount = sizeof(ledPins) / sizeof(ledPins[0]);
bool lampsOn = false;
unsigned long previousTime;
int ledIndex = 0;
bool ledActive = false;

// protoypes
void handleLeds();

// program
void setup() {
    Serial.begin(9600);
    pinMode(buttonPin, INPUT);

    for (const int ledPin : ledPins) {
        pinMode(ledPin, OUTPUT);
    }
}

void loop() {
    // if button is pressed, do shit
    if (digitalRead(buttonPin) == LOW) {
        lampsOn = !lampsOn; // update state
        delay(500);
    }

    const unsigned long currentTime = millis();
    if (lampsOn && currentTime - previousTime > interval) {
        handleLeds();
        previousTime = currentTime;
    }
}

void handleLeds() {
    if (ledActive) {
        digitalWrite(ledPins[ledIndex], LOW);
        Serial.print("Turned LED off: ");
        Serial.println(ledPins[ledIndex]);

        ledIndex = (ledIndex + 1) % ledAmount;
        ledActive = false;
    } else {
        digitalWrite(ledPins[ledIndex], HIGH);
        Serial.print("Turned LED on: ");
        Serial.println(ledPins[ledIndex]);
        ledActive = true;
    }
}