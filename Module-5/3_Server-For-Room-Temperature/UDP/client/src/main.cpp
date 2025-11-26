#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <DHT.h>

// the secret wifi stuff pls dont look or something idk
char ssid[] = "TP-Link_E5CE_5G";
char pass[] = ""; // removed here for obvious reasons
int status = WL_IDLE_STATUS; // wifi connection status

WiFiUDP Udp;

// server stuff
char serverAddr[] = "192.168.1.138";
int serverPort = 8080;

// DHT11 stuff
#define DHTTYPE DHT11
int DHT11_PIN = 7;

DHT DHT11_Sensor(DHT11_PIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    DHT11_Sensor.begin();

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("WiFi module not found :c");
        while (1); // endless loop waow
    }

    // try to connect to wifi network
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);

        delay(5000); // wait for connection ig
    }

    Serial.println("Connected to the WiFi network !! ^~^");

    // initialize udp thingy
    Udp.begin(8080);
}

void loop() {
    // get temperature
    float temperature = DHT11_Sensor.readTemperature();

    // send the packetss
    Udp.beginPacket(serverAddr, serverPort);
    Udp.print(temperature);
    Udp.endPacket();

    Serial.print("Temperature sent: ");
    Serial.println(temperature);

    delay(10000);
}