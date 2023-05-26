<?php

if (isset($_GET["click_type"]) && isset($_GET["controller_id"]) && isset($_GET["wled_ip"])) {
   $click_type = $_GET["click_type"]; // get click type value from HTTP GET
   $controller_id = $_GET["controller_id"]; // get controller ID value from HTTP GET
   $wled_ip = $_GET["wled_ip"]; // get wled_ip value from HTTP GET

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

   $sql = "SELECT json_state, wled_ip FROM WLED_STATE WHERE click_type = '$click_type' AND controller_id = '$controller_id' AND wled_ip = '$wled_ip'";

   $result = $connection->query($sql);

   if ($result->num_rows > 0) {
      $row = $result->fetch_assoc();
      $json_state = $row["json_state"];
      $wled_ip = $row["wled_ip"];
      
      $response = [
         "json_state" => $json_state,
         "wled_ip" => $wled_ip
      ];

      header('Content-Type: application/json');
      echo json_encode($response);
   } else {
      echo "No records found for the given click type, controller ID, and wled_ip";
   }

   $connection->close();
} else {
   echo "One or more parameters are missing in the HTTP request";
}
?>
