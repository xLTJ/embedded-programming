#include <Arduino.h>
#include <avr/boot.h>

// consts
constexpr int Comp2AValue = 156; // 20000 gives us 10 ms between each interrupt (with prescaler 8)
constexpr int Comp1AValue = 15525; // 1s with prescaler 1024
constexpr int Comp1BValue = 46875; // 3s with prescaler 1024
constexpr int PotentiometerPin = 3;
constexpr int ButtonPin = 2;

// prototypes
float readInternalTemp();
void enableTemperatureRead();
void handleButton();

// global vars (the arrays have a bit of extra space justt in case.)
long totalPotentiometerReadings = 0;
int potentiometerReadingCount = 0;
float totalInternalTemperature = 0;
int internalTemperatureCount = 0;

bool readingState = true;
unsigned long previousButtonPress;

// actual stuff
ISR(TIMER2_COMPA_vect) {
    if (!readingState) {
        return;
    }

    OCR2A += Comp2AValue;
    totalPotentiometerReadings += analogRead(PotentiometerPin);
    potentiometerReadingCount++;
}

ISR(TIMER1_COMPA_vect) {
    if (!readingState) {
        return;
    }

    OCR1A += Comp1AValue;
    totalInternalTemperature += readInternalTemp();
    internalTemperatureCount++;
}

ISR(TIMER1_COMPB_vect) {
    if (!readingState) {
        return;
    }

    OCR1B += Comp1BValue;
    Serial.print("Potentiometer: ");
    Serial.println(totalPotentiometerReadings / potentiometerReadingCount);
    Serial.print("Internal Temperature: ");
    Serial.println(totalInternalTemperature / internalTemperatureCount);

    totalPotentiometerReadings = potentiometerReadingCount = internalTemperatureCount = totalInternalTemperature = 0;
}

void setup() {
    Serial.begin(9600);
    pinMode(ButtonPin, INPUT);
    attachInterrupt(0, handleButton, CHANGE);

    // reset timer registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR2A = 0;
    TCCR2B = 0;

    // for timer 2
    TCCR2B |= (1 << CS12) | (1 << CS11) | (1 << CS10); // prescaler = 8 (Set CS10, CS11, CS12 to 1)
    OCR2A = Comp2AValue; // compare value
    TIMSK2 |= (1 << OCIE2A); // enable compare A interrupt

    // for timer 1
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler = 1024
    OCR1A = Comp1AValue;
    OCR1B = Comp1BValue;
    TIMSK1 |= (1 << OCIE1A);
    TIMSK1 |= (1 << OCIE1B);
}

void loop() {
    // bwaa
}

float readInternalTemp() {
    ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);

    // allow ADC reference to settle after changing multiplexer (increased to 10ms)
    delayMicroseconds(10000);

    // ensure ADC is not busy
    while (ADCSRA & _BV(ADSC));

    // start conversion and wait for it to complete
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));

    int adcValue = ADC;

    // get calibration values from signature bytes (datasheet 26.8.10)
    uint8_t ts_offset = boot_signature_byte_get(0x0002);
    uint8_t ts_gain = boot_signature_byte_get(0x0003);

    // datasheet formula approximation
    int32_t temp = adcValue;
    temp -= (273 + 100 - ts_offset);
    temp *= 128;
    temp /= ts_gain;
    temp -= 25;

    return (float)temp;
}

void handleButton() {
    const unsigned long currentTime = millis(); // for checking time since last press (debounce)

    if (digitalRead(ButtonPin) != LOW || currentTime - previousButtonPress < 1000) {
        return;
    }

    readingState = !readingState;
    Serial.print(" --------- Reading state: ");
    Serial.print(readingState == true ? "ON" : "OFF");
    Serial.println("  ---------\n");

    previousButtonPress = currentTime;
}