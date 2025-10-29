#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define LAMP_PIN 7
#define BUTTON_PIN 8

bool lampOn = false;

[[noreturn]] void TaskBlinkLamp(void *_);
[[noreturn]] void TaskCheckButton(void *_);

void setup() {
    pinMode(LAMP_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    Serial.begin(9600);

    // blinking lamp task
    xTaskCreate(
        TaskBlinkLamp,
        "BlinkLamp",
        128,
        nullptr,
        1,
        nullptr
        );

    xTaskCreate(
        TaskCheckButton,
        "TestButton",
        128,
        nullptr,
        1,
        nullptr
        );

    // start tasks
    vTaskStartScheduler();
}

void loop() {
    // tasks instead
}

// basically just flips whatever the lamp output is every 3rd second
[[noreturn]] void TaskBlinkLamp(void *_) {
    for (;;) {
        digitalWrite(LAMP_PIN, lampOn ? LOW : HIGH);
        lampOn = !lampOn;
        vTaskDelay(pdMS_TO_TICKS(2000)); // wait 3 sec (convert to ticks)
    }
}

void TaskCheckButton(void *_) {
    for (;;) {
        Serial.println(digitalRead(BUTTON_PIN) ? "botton not pressed" : "button pressed");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
