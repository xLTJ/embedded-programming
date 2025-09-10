#include <Arduino.h>

constexpr int ledPins[] = {12, 11, 10, 9};
constexpr  int buttonPin = 2;
constexpr long interval = 100;

enum state {
    Idle,
    Running,
    Stopped,
};

// other variables
constexpr int ledAmount = sizeof(ledPins) / sizeof(ledPins[0]);
unsigned long previousTime;
int ledIndex = 0;
state currentState = Idle;

// protoypes
void handleLeds();
void handleButton();

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
        handleButton();
        delay(500);
    }

    const unsigned long currentTime = millis();
    if (currentState == Running && currentTime - previousTime > interval) {
        handleLeds();
        previousTime = currentTime;
    }
}

void handleLeds() {
    digitalWrite(ledPins[ledIndex], LOW);
    Serial.print("Turned LED off: ");
    Serial.println(ledPins[ledIndex]);

    ledIndex = (ledIndex + 1) % ledAmount;
    digitalWrite(ledPins[ledIndex], HIGH);
    Serial.print("Turned LED on: ");
    Serial.println(ledPins[ledIndex]);
}

void handleButton() {
    switch (currentState) {
        case Stopped:
            digitalWrite(ledPins[ledIndex], LOW);
            currentState = Idle;
            break;
        case Idle:
            currentState = Running;
            break;
        case Running:
            currentState = Stopped;
            break;
    }
}
