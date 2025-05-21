#include "mqtt_server.h"
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <Arduino.h>

WiFiClient espClient;
PubSubClient client(espClient);

//Set up Wifi
void setup_wifi(const char* ssid, const char* password){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWifi connected. IP address: ");
    Serial.println(WiFi.localIP());
}

//Set up MQTT
void mqqt_setup(const char* mqtt_server){
    client.setServer(mqtt_server, 1883);
}

//mqtt reconnect and loop
void mqtt_loop() {
    if (!client.connected()) {
        client.connect("ESPClient");
    }
    client.loop();
}