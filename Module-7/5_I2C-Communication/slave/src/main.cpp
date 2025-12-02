#include <Arduino.h>
#include <Wire.h>

// consts -------------------------
constexpr int LED_PIN = 13;
constexpr int POT_PIN = A0;

// prototypes ------------------------
void receiveEvent(int);
void requestEvent();

// main code type shit --------------
void setup() {
    pinMode(LED_PIN, OUTPUT);
    Wire.begin(0x08);              // I2C address 0x08
    Wire.onReceive(receiveEvent);  // register receive handler
    Wire.onRequest(requestEvent);  // register request handler
}

void loop() {
    // nothing lol
}

// when raspberry pi writes data
void receiveEvent(int bytes) {
    if (Wire.available()) {
        char command = Wire.read();

        if (command == '1') {
            digitalWrite(LED_PIN, HIGH);
        } else if (command == '0') {
            digitalWrite(LED_PIN, LOW);
        }
    }
}

// when raspberry pi requests data
void requestEvent() {
    // pls dont make me explain ts, 99% i wont remember ;~;
    int potValue = analogRead(POT_PIN);  // read 0-1023
    byte highByte = potValue >> 8;       // upper 8 bits
    byte lowByte = potValue & 0xFF;      // lower 8 bits

    Wire.write(highByte);
    Wire.write(lowByte);
}
