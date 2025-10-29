#include <Arduino.h>
#include <avr/boot.h>
float readInternalTemp();
void enableTemperatureRead();

void setup() {
    Serial.begin(9600);

    enableTemperatureRead();
    Serial.println("Reading internal temperature...");
}

void loop() {
    Serial.println(readInternalTemp());
    delay(100);
}

void enableTemperatureRead() {
    // select ADC channel 8 (temperature sensor) and 1.1V internal reference
    ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
}

float readInternalTemp() {
    // start conversion, and wait for it to complete
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));

    int adcValue = ADC;

    // get calibration values. See datasheet 26.8.10 for this
    uint8_t ts_offset = boot_signature_byte_get(0x0002);
    uint8_t ts_gain = boot_signature_byte_get(0x0003);

    // use datasheet formula thingy
    int32_t temp = adcValue;
    temp -= (273 + 100 - ts_offset);
    temp *= 128;
    temp /= ts_gain;
    temp -= 25;

    return (float)temp;
}

