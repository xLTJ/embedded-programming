#include <Arduino.h>
void setup() {
    Serial.begin(9600); // initialize serial communication (for printing to console)
    pinMode(LED_BUILTIN, OUTPUT); // Set buildin LED as output so we can make it blink
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH); // LED on
    Serial.println("Hello World!"); // print to console
    delay(1000);

    digitalWrite(LED_BUILTIN, LOW); // LED off
    delay(1000);
}