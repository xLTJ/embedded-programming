#include <Arduino.h>

constexpr int ledPins[] = {12, 11, 10, 9};
constexpr int ledPointValues[] = {-15, -10, -5, 10};
constexpr int targetLedPin = 9;
constexpr char targetColor[] = "blue";

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
int points = 0;
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

    Serial.println("Welcome to LED strip gambling simulator (woahhh)");
    Serial.print("Stop on the ");
    Serial.print(targetColor);
    Serial.println(" LED to get points. Any other LED's make you lose points (bad)");
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

    ledIndex = (ledIndex + 1) % ledAmount;
    digitalWrite(ledPins[ledIndex], HIGH);
}

void handleButton() {
    switch (currentState) {
        case Stopped:
            digitalWrite(ledPins[ledIndex], LOW);
            currentState = Idle;
            break;
        case Idle:
            currentState = Running;
            Serial.println("The LED's are running...");
            break;
        case Running:
            currentState = Stopped;
            Serial.print("You hit LED: ");
            Serial.println(ledIndex);
            if (ledPins[ledIndex] == targetLedPin) {
                Serial.println("This is the target color !!");
                Serial.print("You get awarded points, omg: +");
            } else {
                Serial.println("This is not the target, you suck, :thumbs-down:");
                Serial.print("You lose points: ");
            }
            points += ledPointValues[ledIndex];
            Serial.println(ledPointValues[ledIndex]);
            Serial.print("You now have: ");
            Serial.print(points);
            Serial.println(" points");
            break;
    }
}
