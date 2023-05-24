#include <Arduino.h>
#include <RCSwitch.h>
#include "wifi.h"
#include "wled_api.h"
#include "mysql_api.h"
#include "wled.h"

HTTPClient http;
WiFiClient client;
RCSwitch my_switch = RCSwitch();

#define FLASH_BUTTON_PIN 0
#define RC_RECEIVE_PIN 4

unsigned long last_time = 0;
static uint32_t next_heap_millis = 0;
const unsigned long register_mode_duration = 30000;  // 20 seconds in milliseconds
const unsigned long blink_interval = 1000; // Time interval in milliseconds
unsigned long previous_blink_time = 0; // Variable to store the previous blink time
unsigned long retgister_mode_start_time = 0;  // Variable to store the start time
int new_click_type = -1;
unsigned long int new_controller_id = -1;
bool register_mode = false;

enum click_type {
  long_click = 0,
  single_click = 1
};

void setup() {
  Serial.begin(115200);
  wifi_connect();
  //homekit_storage_reset();
  my_switch.enableReceive(RC_RECEIVE_PIN);
  pinMode(BUILTIN_LED, OUTPUT); // Set the LED pin as output
  pinMode(FLASH_BUTTON_PIN, INPUT_PULLUP);  // Configure the pin as input with internal pull-up resistor
}

void get_and_save_state(String ip, unsigned long int controller_id, int click_type) {
  String json_state = get_wled_state(ip);
  Serial.println(json_state);

  wled myWLED;
  myWLED.wled_ip = ip;
  myWLED.json_state = json_state;
  myWLED.click_type = click_type;
  myWLED.controller_id = controller_id;

  String db_insert_response = db_insert_wled_state(myWLED);
}

void get_and_save_states(unsigned long int controller_id, int click_type) {
  int num_ips = sizeof(wled_ips) / sizeof(wled_ips[0]);
  for (int i = 0; i < num_ips; i++) {
    get_and_save_state(wled_ips[i], controller_id, click_type);
    if (i != num_ips - 1) {
      delay(10000);
    }
  }
}

void blink_led(){
  unsigned long current_millis = millis(); // Get the current time

  if (current_millis - previous_blink_time >= blink_interval) {
    previous_blink_time = current_millis; // Update the previous blink time

    // Toggle the LED state
    if (digitalRead(BUILTIN_LED) == HIGH) {
      digitalWrite(BUILTIN_LED, LOW);
    } else {
      digitalWrite(BUILTIN_LED, HIGH);
    }
  }
}

void register_button() {
  blink_led();

  if (my_switch.available()) {
    unsigned long current_time = millis();
    unsigned long time_difference = current_time - last_time;

    if (time_difference < 500) {
      Serial.println("long click");
      Serial.println(time_difference);
      new_click_type = long_click;
      new_controller_id = my_switch.getReceivedValue();
    } else {
      Serial.println("single click");
      Serial.println(time_difference);
      new_click_type = single_click;
      new_controller_id = my_switch.getReceivedValue();
    }
    
    last_time = current_time;
    my_switch.resetAvailable();
  }
}

void loop() {
  int button_state = digitalRead(FLASH_BUTTON_PIN);

  if(register_mode){
    register_button();
  }

  if (button_state == LOW && !register_mode) {
    Serial.println("register enabled");
    retgister_mode_start_time = millis();
    register_mode = true;
  }

  if (register_mode && (millis() - retgister_mode_start_time >= register_mode_duration)) {
    Serial.println("register mode disabled");
    if(new_controller_id != -1 && new_click_type != -1) {
      digitalWrite(BUILTIN_LED, HIGH);
      get_and_save_states(new_controller_id, new_click_type);
      new_controller_id = -1;
      new_click_type = -1;
    }
    digitalWrite(BUILTIN_LED, LOW);
    register_mode = false;
  }
}