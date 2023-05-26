# ESP8266 WLED Controller

The ESP8266 WLED Controller is a program that enables the control of multiple WLED (Wireless LED) devices using an Arduino board and a 433MHz receiver module. This project was only tested with 433MHz remote control switches.

## Features

- Control multiple WLED devices simultaneously
- Register different states for each WLED device based on a received 433MHz signals
- Each signal can set two sets of states, one for short and one for long signals
- Store and retrieve WLED states in a MySQL database
- Update the state of WLED devices via HTTP requests

## Getting Started

### Prerequisites

To run the WLED Controller, you will need the following:

- ESP8266 board
- 433MHz receiver module
- 433MHz remote control button(s)
- A web server to host the database (for storing WLED states)
- WLED devices connected to the network

### Configuration

Before running the WLED Controller, you need to configure the following:

- Rename `network_setting_example.h` to `network_setting.h`
- Wi-Fi credentials: Set the ssid and password inside network_setting.h
- WLED IP addresses: Modify the `wled_ips` array in network_setting.h to include the IP addresses of your WLED devices
- MySQL database: Set the `mysql_ip` to the ip of the server hosting your database

### Usage
1. Connect the 433MHz receiver module to the ESP8266 according to the pin assignment specified in the code.
2. Power on the ESP8266. The ESP8266 WLED Controller will start running.
3. Whenever a a signal is received, the program will check if the click type and controller ID has states registered in the database. If it does, the states will be set.

### Registration Mode
1. Press the flash button on the ESP8266 to enable the 30 seconds registration mode. The LED on the ESP8266 will blink during the 30 seconds.
2. During registration mode, use your controller to send the click type and controller ID to the receiver.
3. After the 30 seconds (when the LED stops blinking), the states of all the WLED device will be saved in the MySQL database based on the click type and controller ID obtained during registration mode.

## Acknowledgments

- To create a WLED device see [The WLED project](https://github.com/Aircoookie/WLED)
- I used a XAMPP MySQL server on my local network for this project. This tutorial was helpful: [ESP32 - MySQL](https://esp32io.com/tutorials/esp32-mysql)




