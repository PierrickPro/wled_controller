#include <Arduino.h>
#include <RCSwitch.h>
#include "wifi.h"
#include "wled_api.h"
#include "mysql_api.h"
#include "wled.h"

RCSwitch my_switch = RCSwitch();

HTTPClient http;
WiFiClient client;

unsigned long last_time = 0;
static uint32_t next_heap_millis = 0;

void rc_button_loop() {
  if (my_switch.available()) {
    unsigned long current_time = millis();
    unsigned long time_difference = current_time - last_time;
    uint8_t cha_value = 0;

    if (time_difference < 500) {
      Serial.println("Long click");
      Serial.println(time_difference);
      // TODO call wled api
    } else {
      Serial.println("Single click");
      Serial.println(time_difference);
      // TODO call wled api
    }
    
    last_time = current_time;
    
    my_switch.resetAvailable();
  }
}

void setup() {
    Serial.begin(115200);
    wifi_connect();
    //homekit_storage_reset();
    my_switch.enableReceive(2);
}

void loop() {
  rc_button_loop();
}