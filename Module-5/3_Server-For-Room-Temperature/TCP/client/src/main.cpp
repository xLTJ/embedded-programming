#include <Arduino.h>
#include <WiFiNINA.h>
#include <DHT.h>

// the secret wifi stuff pls dont look or something idk
char ssid[] = "TP-Link_E5CE_5G";
char pass[] = ""; // removed here for obvious reasons
int status = WL_IDLE_STATUS; // wifi connection status

WiFiClient client;

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
}

void loop() {
    // get temperature
    float temperature = DHT11_Sensor.readTemperature();

    // connect to TCP server
    if (client.connect(serverAddr, serverPort)) {
        Serial.println("Connected to server");

        // send the temperature
        client.print(temperature);
        Serial.print("Temperature sent: ");
        Serial.println(temperature);

        client.stop();
    } else {
        Serial.println("Connection failed ;~;");
    }

    delay(10000);
}