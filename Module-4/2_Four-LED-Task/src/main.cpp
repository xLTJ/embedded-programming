#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

constexpr int StartDelay = 2;
constexpr int DelayIncrement = 1;
constexpr int LedPins[] = {7, 8, 9, 10};

constexpr int LedAmount = sizeof(LedPins) / sizeof(LedPins[0]);

struct TaskBlinkLampParams {
    int ledPin;
    int blinkDelay;
};

[[noreturn]] void TaskBlinkLamp(void *pvParameters);

void setup() {
    Serial.begin(9600);

    int index = 0;
    for (const int pin: LedPins) {
        pinMode(pin, OUTPUT);
        auto* params = new TaskBlinkLampParams{pin, StartDelay + index * DelayIncrement};

        xTaskCreate(
            TaskBlinkLamp,
            "BlinkLamp",
            128,
            params,
            1,
            nullptr
        );

        index++;
    }

    vTaskStartScheduler();
}

void loop() {
    // no
}

[[noreturn]] void TaskBlinkLamp(void *pvParameters) {
    const auto params = (TaskBlinkLampParams*)pvParameters;

    for (;;) {
        digitalWrite(params->ledPin, HIGH);
        vTaskDelay(pdMS_TO_TICKS(params->blinkDelay * 1000));
        digitalWrite(params->ledPin, LOW);
        vTaskDelay(pdMS_TO_TICKS(params->blinkDelay * 1000));
    }
}