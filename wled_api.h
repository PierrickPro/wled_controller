#ifndef WLED_API
#define WLED_API

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "wled.h"

extern HTTPClient http;
extern WiFiClient client;

String get_wled_state(String wled_ip) {
  Serial.println("GET wled state");
  String url = "http://" + wled_ip + "/json/state";

  http.begin(client, url);
  int httpCode = http.GET();

  String payload = "";
  if (httpCode > 0) {
    payload = http.getString();
  }
  else {
    Serial.println("Error on HTTP GET request");
  }

  http.end();
  return payload;
}

String post_wled_state(String wled_ip, String json_state) {
  Serial.println("POST wled state");
  String url = "http://" + wled_ip + "/json/state";

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json_state);

  String payload = "";
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("POST Request Code " + httpCode);
  }
  else {
    Serial.println("Error on HTTP POST request");
  }

  http.end();
  return payload;
}

#endif // WLED_API