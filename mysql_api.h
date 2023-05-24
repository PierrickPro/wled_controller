#ifndef MYSQL_API
#define MYSQL_API

#include <ESP8266WiFi.h>
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
  if (http_code > 0) {
    Serial.println(url);
    response = http.getString();
    Serial.println("GET Request Code: " + String(http_code));
  }
  else {
    Serial.println("Error on HTTP GET request");
  }

  http.end();
  return response;
}

wled db_get_wled_state_and_ip(unsigned long int controller_id, int click_type) {
  Serial.println("Getting wled state from db");
  
  String get_wled_state_and_ip_query = "get_wled_state_and_ip.php";
  String query_params = "&click_type=" + String(click_type) +
                        "&controller_id=" + String(controller_id);
  String url = mysql_ip + "/" + get_wled_state_and_ip_query + "?" + query_params;

  http.begin(client, url);
  int http_code = http.GET();

  wled my_wled;

  if (http_code > 0) {
    Serial.println("GET Request Code: " + String(http_code));

    String json_payload = http.getString();
    DynamicJsonDocument doc(600);
    DeserializationError error = deserializeJson(doc, json_payload);

    if (error) {
      Serial.print("Error parsing failed: ");
      Serial.println(error.c_str());
      http.end();
      return my_wled;
    }

    // Extract variables from the JSON document
    String json_state = doc["json_state"];
    String wled_ip = doc["wled_ip"];

    my_wled.json_state = json_state;
    my_wled.wled_ip = wled_ip;
    my_wled.click_type = click_type;
    my_wled.controller_id = controller_id;
  } else {
    Serial.println("Error on HTTP GET request");
  }

  http.end();
  return my_wled;
}

#endif // MYSQL_API