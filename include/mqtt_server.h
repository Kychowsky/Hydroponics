#include <ESP8266WiFi.h>

#ifndef MQTT_SERVER_H
#define MQTT_SERVER_H


void setup_wifi(const char* ssid, const char* password);
void mqqt_setup(const char* mqtt_server);
void mqtt_loop();

#endif