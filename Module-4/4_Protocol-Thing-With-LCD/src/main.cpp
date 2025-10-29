#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include <LiquidCrystal.h>

// --------------------- CONFIGS ------------------------------------
constexpr int LedPin = 7;
constexpr int ButtonPin = 8;
constexpr int rs = 11,
en = 12,
d4 = 2,
d5 = 3,
d6 = 4,
d7 = 5;

constexpr int LCDBufferSize = 17; // one line + null terminator

// --------------------- Global vars and stuff -----------------------
QueueHandle_t QueueDelayChange;
QueueHandle_t QueueButtonInstruction;
QueueHandle_t QueueButtonResponse;
QueueHandle_t QueueLCDWrite;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

enum ButtonInstruction {
    SAVE,
    SEND,
};

[[noreturn]] void taskReadInput(void *_);
[[noreturn]] void taskBlinkLamp(void *_);
[[noreturn]] void taskButton(void *_);
[[noreturn]] void taskWriteToLCD(void *_);

// ---------------------- Main code --------------------------------------

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    QueueDelayChange = xQueueCreate(1, sizeof(int));
    QueueButtonInstruction = xQueueCreate(2, sizeof(int));
    QueueButtonResponse = xQueueCreate(1, sizeof(unsigned long));
    QueueLCDWrite = xQueueCreate(1, LCDBufferSize);

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
        64,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        taskButton,
        "Button",
        64,
        nullptr,
        1,
        nullptr
    );

    xTaskCreate(
        taskWriteToLCD,
        "WriteToLCD",
        128,
        nullptr,
        1,
        nullptr
    );

    Serial.println("All task created");
    vTaskStartScheduler();
}

void loop() {
	// no
}

// --------------------------------- Tasks ---------------------------------

[[noreturn]] void taskReadInput(void *_) {
    for (;;) {
        if (Serial.available() == 0) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        char input[LCDBufferSize];
        const size_t bytesRead = Serial.readBytesUntil('\n', input, sizeof input - 1);
        input[bytesRead] = '\0';
        const char commandChar = input[0];

        switch (commandChar) {
            case 'A': {
                char inputNumber[2];
                strncpy(inputNumber, input + 2, 2);
                const int newDelay = atoi(inputNumber);

                if (newDelay < 1 || newDelay > 99 || input[1] != ':') {
                    Serial.println("Invalid Input for command A. Use format `A:xx`");
                    break;
                }

                Serial.print("New Delay: ");
                Serial.println(newDelay);
                xQueueSend(QueueDelayChange, &newDelay, portMAX_DELAY);
                break;
            }
            case 'B': {
                ButtonInstruction instruction = SEND;
                xQueueSend(QueueButtonInstruction, &instruction, portMAX_DELAY);
                int response;
                xQueueReceive(QueueButtonResponse, &response, portMAX_DELAY);
                Serial.print("Time since last button press: ");
                Serial.print(response / 1000);
                Serial.println(" seconds");
                break;
            }
            case 'C': {
                if (input[1] != ':') {
                    Serial.println("Invalid input for command C. use format `C:<text>`");
                    break;
                }

                char toLCD[LCDBufferSize] = {0};
                strncpy(toLCD, input + 2, LCDBufferSize);
                toLCD[16] = '\0';
                Serial.print("Writing to LCD: ");
                Serial.println(toLCD);
                xQueueSend(QueueLCDWrite, toLCD, portMAX_DELAY);
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
        xQueueReceive(QueueDelayChange, &blinkDelay, 0);

        digitalWrite(LedPin, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LedPin, LOW);
        vTaskDelay(pdMS_TO_TICKS(blinkDelay * 1000));
    }
}

// buttonHandler and buttonPoller had to be combined into one to save RAM
[[noreturn]] void taskButton(void *_) {
    unsigned long lastPress = millis();
    bool lastState = HIGH;
    constexpr int debounce = 50;
    constexpr int pollInterval = 100;

    for (;;) {
        // Poll button state
        const bool currentState = digitalRead(ButtonPin);

        if (lastState == HIGH && currentState == LOW) {
            vTaskDelay(pdMS_TO_TICKS(debounce));
            lastPress = millis();
        }

        lastState = currentState;

        // Check for SEND instruction with timeout
        ButtonInstruction instruction;
        if (xQueueReceive(QueueButtonInstruction, &instruction, pdMS_TO_TICKS(pollInterval)) == pdTRUE) {
            if (instruction == SEND) {
                unsigned long timeSinceLastPress = millis() - lastPress;
                xQueueSend(QueueButtonResponse, &timeSinceLastPress, portMAX_DELAY);
            }
        }
    }
}


[[noreturn]] void taskWriteToLCD(void *_) {
    for (;;) {
        char output[LCDBufferSize];
        xQueueReceive(QueueLCDWrite, &output, portMAX_DELAY);
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print(output);
    }
}