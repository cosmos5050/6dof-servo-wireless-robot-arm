#include <WiFi.h>
#include "ServoControl.h"

const char* ssid = "ESP32_Server";
// Pass has to be >8 chars long for some reason
const char* password = "12345678";

WiFiServer server(5000);

void setup() {
  Serial.begin(9600);

  // Create own Wi-Fi network for controller ESP32 to join
  WiFi.softAP(ssid, password);

  Serial.print("Server IP: ");
  Serial.println(WiFi.softAPIP());

  // Start TCP server
  server.begin();
  Serial.println("TCP server started");

  // Initialize I2C communication
  initServoControl();
}

void loop() {
  WiFiClient client = server.available();

  // Wait for client to connect
  while (!client) {
    client = server.available();
    Serial.println("...");    
    delay(200);
  }

  Serial.println("Client connected");

  int currentServoAngle = 90;
  setServoAngle(0, currentServoAngle);

  while (client.connected()) {

    if (client.available()) {

      String receivedData = client.readStringUntil('\n');
      receivedData.trim();

      if (receivedData == "right") {
        currentServoAngle = constrain(currentServoAngle + 5, 0, 180);
      }
      else if (receivedData == "left") {
        currentServoAngle = constrain(currentServoAngle - 5, 0, 180);
      }

setServoAngle(0, currentServoAngle);

      setServoAngle(0, currentServoAngle);
    }

    delay(100);
  }

  client.stop();
  Serial.println("Client disconnected");
}