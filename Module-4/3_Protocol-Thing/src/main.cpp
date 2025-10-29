#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "queue.h"

constexpr int LedPin = 7;
constexpr int ButtonPin = 8;

QueueHandle_t DelayChangeQueue = xQueueCreate(10, sizeof(int));
QueueHandle_t ButtonInstructionQueue = xQueueCreate(10, sizeof(int));
QueueHandle_t ButtonResponseQueue = xQueueCreate(10,  sizeof(int));

enum ButtonInstruction {
    SAVE,
    SEND,
};

[[noreturn]] void taskReadInput(void *_);
[[noreturn]] void taskBlinkLamp(void *_);
[[noreturn]] void taskButtonHandler(void *_);
[[noreturn]] void taskButtonPoller(void *_);

void setup() {
    Serial.begin(9600);

    pinMode(LedPin, OUTPUT);
    pinMode(ButtonPin, INPUT_PULLUP);

    xTaskCreate(
        taskReadInput,
        "ReadInput",
        128,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        taskBlinkLamp,
        "BlinkLamp",
        128,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        taskButtonHandler,
        "ButtonHandler",
        128,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        taskButtonPoller,
        "ButtonPoller",
        128,
        nullptr,
        1,
        nullptr
        );

    vTaskStartScheduler();
}

void loop() {
// write your code here
}

[[noreturn]] void taskReadInput(void *_) {
    for (;;) {
        if (Serial.available() == 0) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        String input = Serial.readString();
        const char commandChar = input.charAt(0);

        switch (commandChar) {
            case 'A': {
                String inputNumber = input.substring(2, input.length());
                const int newDelay = atoi(inputNumber.c_str());

                if (newDelay < 1 || newDelay > 99 || input.charAt(1) != ':') {
                    Serial.println("Invalid Input for command A. Use format `A:xx`");
                    break;
                }

                Serial.print("New Delay: ");
                Serial.println(newDelay);
                xQueueSend(DelayChangeQueue, &newDelay, portMAX_DELAY);
                break;
            }
            case 'B': {
                ButtonInstruction instruction = SEND;
                xQueueSend(ButtonInstructionQueue, &instruction, portMAX_DELAY);
                int response;
                xQueueReceive(ButtonResponseQueue, &response, portMAX_DELAY);
                Serial.print("Time since last button press: ");
                Serial.print(response / 1000);
                Serial.println(" seconds");
                break;
            }
            default: {
                break;
            }
        }
    }
}

[[noreturn]] void taskBlinkLamp(void *_) {
    int blinkDelay = 1; // in seconds

    for (;;) {
        xQueueReceive(DelayChangeQueue, &blinkDelay, 0);

        digitalWrite(LedPin, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LedPin, LOW);
        vTaskDelay(pdMS_TO_TICKS(blinkDelay * 1000));
    }
}

[[noreturn]] void taskButtonHandler(void *_) {
    unsigned long lastPress = millis();

    for (;;) {
        ButtonInstruction instruction;
        xQueueReceive(ButtonInstructionQueue, &instruction, portMAX_DELAY);

        switch (instruction) {
            case SAVE: {
                lastPress = millis();
                break;
            }
            case SEND: {
                unsigned long timeSinceLastPress = millis() - lastPress;
                xQueueSend(ButtonResponseQueue, &timeSinceLastPress, portMAX_DELAY);
                break;
            }
        }
    }
}

[[noreturn]] void taskButtonPoller(void *_) {
    bool lastState = HIGH;
    constexpr int debounce = 50;

    for (;;) {
        const bool currentState = digitalRead(ButtonPin);

        if (lastState == HIGH && currentState == LOW) {
            vTaskDelay(pdMS_TO_TICKS(debounce));
            ButtonInstruction instruction = SAVE;
            xQueueSend(ButtonInstructionQueue, &instruction, portMAX_DELAY);
        }

        lastState = currentState;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}