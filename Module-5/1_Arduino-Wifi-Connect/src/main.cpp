#include <Arduino.h>
#include <WiFiNINA.h>

// the secret stuff pls dont look or something idk
char ssid[] = "TP-Link_E5CE_5G";
char pass[] = ""; // removed here for obvious reasons
int status = WL_IDLE_STATUS; // wifi connection status

// enter what u wanna ping
String hostNameToPing = "bing.com";

void setup() {
    Serial.begin(9600);

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
    // pinging time
    Serial.print("pinging ");
    Serial.print(hostNameToPing);
    Serial.println("...");

    int pingResult = WiFi.ping(hostNameToPing);

    // if result is over 0, the host was reached and the result is the RTT
    // if its under 0, it was unreachable, and the result is the error code
    if (pingResult >= 0) {
        Serial.print("Host reached, RTT = ");
        Serial.print(pingResult);
        Serial.println(" ms");
    } else {
        Serial.println("Host unreachable, error code: ");
        Serial.println(pingResult);
    }

    delay(2000);
}