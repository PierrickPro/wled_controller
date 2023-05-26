#ifndef MYSQL_API
#define MYSQL_API

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "wled.h"

extern HTTPClient http;
extern WiFiClient client;

String db_insert_wled_state(wled data) {
  Serial.println("inserting wled state into db");
  
  String insert_wled_state_query = "insert_wled_state.php";

  String query_params = "wled_ip=" + data.wled_ip +
                        "&json_state=" + data.json_state +
                        "&click_type=" + String(data.click_type) +
                        "&controller_id=" + String(data.controller_id);
  
  String url = mysql_ip + "/" + insert_wled_state_query + "?" + query_params;

  http.begin(client, url);
  int http_code = http.GET();

  String response = "";
  if (http_code == 200) {
    response = http.getString();
    Serial.println("GET request code: " + String(http_code));
  }
  else if (http_code > 0) {
    Serial.println("GET request error code: " + String(http_code));
  }
  else {
    Serial.println("Error on HTTP GET request");
  }

  http.end();
  return response;
}

String db_get_wled_state(String wled_ip, unsigned long int controller_id, int click_type) {
  Serial.println("Getting wled state from db");
  
  String get_wled_state_and_ip_query = "get_wled_state.php";
  String query_params = "wled_ip=" + wled_ip + 
                        "&click_type=" + String(click_type) +
                        "&controller_id=" + String(controller_id);
  String url = mysql_ip + "/" + get_wled_state_and_ip_query + "?" + query_params;

  http.begin(client, url);
  int http_code = http.GET();

  String payload = "";

  if (http_code == 200) {
    Serial.println("GET request code: " + String(http_code));

    payload = http.getString();

    DynamicJsonDocument doc(600);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("Error parsing failed: ");
      Serial.println(error.c_str());
      http.end();
      return "";
    }

    // Extract variables from the JSON document
    payload = doc["json_state"].as<String>();

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

#endif // MYSQL_API