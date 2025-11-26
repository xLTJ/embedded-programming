#include <Arduino.h>
#include <WiFiNINA.h>

void setup() {
    Serial.begin(9600);

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("WiFi module not found :c");
        while (1); // endless loop waow
    }
}

void loop() {
    // scan for networks
    Serial.println("Scanning for networks...");

    int ssidAmount = WiFi.scanNetworks(); // how many ssid's?
    if (ssidAmount == -1) {
        Serial.println("couldn't get a wifi connection :c");
    }

    // print list of networks
    Serial.print(ssidAmount);
    Serial.println(" networks found");

    // for each, print basic info
    for (int i = 0; i < ssidAmount; i++) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(WiFi.SSID(i));
        Serial.print("\tSignal: ");
        Serial.print(WiFi.RSSI(i));
        Serial.print(" dBm");
    }

    delay(10000);
}