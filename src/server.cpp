#include "server.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// — Wi-Fi creds —
static const char* ssid     = "lukasz";
static const char* password = "kangaroos";

// — MQTT broker on same LAN —
static const char*  mqtt_host = "10.0.0.249";
static const uint16_t mqtt_port = 1883;

// — Client ID + topic names —
static const char* client_id        = "HydroClient";
static const char* tdsSensorTopic   = "home/sensors/tds";
static const char* tempSensorTopic  = "home/sensors/temp";
static const char* pHSensorTopic    = "home/sensors/ph";
static const char* phDownActuatorTopic    = "home/actuator/phdown";  // wildcard for any actuator command

WiFiClient   net;
PubSubClient mqtt(net);

void callback(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];
  Serial.printf("Recv [%s] → %s\n", topic, msg.c_str());

  // // Example: if you later hook up a pump, valve, etc.
  // if (String(topic).startsWith("home/actuator/")) {
  //   String device = String(topic).substring(strlen("home/actuator/"));
  //   if (device == "pump") {
  //     if (msg == "ON")  digitalWrite(PUMP_PIN, HIGH);
  //     if (msg == "OFF") digitalWrite(PUMP_PIN, LOW);
  //   }
  //   // add more devices here...
  // }
}

void setupServer() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("\nWi-Fi OK, IP=" + WiFi.localIP().toString());

  mqtt.setServer(mqtt_host, mqtt_port);
  mqtt.setCallback(callback);

  // initial connection & subscribe actuator commands
  while (!mqtt.connected()) {
    Serial.print("Connecting MQTT… ");
    if (mqtt.connect(client_id)) {
      Serial.println("connected");
      mqtt.subscribe(phDownActuatorTopic);
    } else {
      Serial.printf("fail, rc=%d; retry in 2s\n", mqtt.state());
      delay(2000);
    }
  }
}

void loopServer() {
  if (!mqtt.connected()) {
    // reconnect logic here
    while (!mqtt.connected()) {
      Serial.print("Reconnecting MQTT… ");
      if (mqtt.connect(client_id)) {
        Serial.println("reconnected");
        mqtt.subscribe(pHSensorTopic);
      } else {
        Serial.printf("fail, rc=%d; retry in 2s\n", mqtt.state());
        delay(2000);
      }
    }
  }
  mqtt.loop();

  // publish sensor readings every 10s
  static unsigned long last = millis();
  if (millis() - last > 10000) {
    //Water Level Sensor

    float tdsSensorReading = getValue_tdsSensor();
    float temp = readTempC();
    setTemp_tdsSensor(temp);
    update_tdsSensor();
    float tds  = getValue_tdsSensor();  
    
    //float ph   = readpH();   

    mqtt.publish(tdsSensorTopic,  String(tds).c_str());
    mqtt.publish(tempSensorTopic, String(temp).c_str());
    //mqtt.publish(pHSensorTopic,   String(ph).c_str());

    Serial.printf("Published TDS=%.2f, Temp=%.2f", tds, temp);
    last = millis();
  }
}

void checkWifiStatus(){
  if(WiFi.status() != WL_CONNECTED)
  reconnectWifi();
}

void reconnectWifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("\nWi-Fi OK, IP=" + WiFi.localIP().toString());
}

