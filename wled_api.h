#ifndef WLED_API
#define WLED_API

#include <ESP8266HTTPClient.h>
#include "wled.h"

extern HTTPClient http;
extern WiFiClient client;

String get_wled_state(String wled_ip) {
  Serial.println("GET wled state");
  String url = "http://" + wled_ip + "/json/state";

  http.begin(client, url);
  int http_code = http.GET();

  String payload = "";
  if (http_code == 200) {
    Serial.println("GET request code " + String(http_code));
    payload = http.getString();
  }
  else if (http_code > 0) {
    Serial.println("GET request error code: " + String(http_code));
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
  int http_code = http.POST(json_state);

  String payload = "";
  if (http_code == 200) {
    Serial.println("POST request code: " + String(http_code));
    payload = http.getString();
  }
  else if (http_code > 0) {
    Serial.println("POST request error code: " + String(http_code));
    Serial.println(http.getString());
  }
  else {
    Serial.println("Error on HTTP POST request");
  }

  http.end();
  return payload;
}

#endif // WLED_API