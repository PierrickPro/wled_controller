<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

if (isset($_GET["wled_ip"]) && isset($_GET["json_state"]) && isset($_GET["click_type"]) && isset($_GET["controller_id"])) {
   $wled_ip = $_GET["wled_ip"]; // get IP value from HTTP GET
   $json_state = $_GET["json_state"]; // get JSON state value from HTTP GET
   $click_type = $_GET["click_type"]; // get click type value from HTTP GET
   $controller_id = $_GET["controller_id"]; // get controller ID value from HTTP GET

   $servername = "localhost";
   $username = "esp8266";
   $password = "password";
   $database_name = "database_esp8266";

   // Create MySQL connection from PHP to MySQL server
   $connection = new mysqli($servername, $username, $password, $database_name);
   // Check connection
   if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
   }

   $sql = "INSERT INTO WLED_STATE (wled_ip, json_state, click_type, controller_id)
           VALUES ('$wled_ip', '$json_state', $click_type, '$controller_id')
           ON DUPLICATE KEY UPDATE
           wled_ip = VALUES(wled_ip),
           json_state = VALUES(json_state),
           click_type = VALUES(click_type),
           controller_id = VALUES(controller_id)";

   if ($connection->query($sql) === TRUE) {
      echo "Record created or updated successfully";
   } else {
      echo "Error: " . $sql . " => " . $connection->error;
   }

   $connection->close();
} else {
   echo "One or more parameters are missing in the HTTP request";
}
?>