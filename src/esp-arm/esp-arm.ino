#include <WiFi.h>
#include "ServoControl.h"

const char* ssid = "ESP32_Server";
// Pass has to be >8 chars long for some reason
const char* password = "12345678";
const int angleIncrementVal = 5;

WiFiServer server(5000);

void setup() {
  Serial.begin(115200);

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

  // Array for servo angles
  int servoAngleArr[6] = {90, 90, 90, 90, 90, 90};

  while (client.connected()) {
    if (client.available()) {
      // Store data transmitted from client/controller
      String packet = client.readStringUntil('\n');
      // Remove whitespace and end and start
      packet.trim();
      Serial.println(packet);

      // Decode packet
      for (int i = 0; i < packet.length(); i+=2) {
        switch (packet[i]) {
          case 'A':
            if (packet[i+1] == '0') {
              servoAngleArr[0] = constrain(servoAngleArr[0] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[0] = constrain(servoAngleArr[0] - angleIncrementVal, 0, 180);
            }
            break;
          case 'B':
            if (packet[i+1] == '0') {
              servoAngleArr[1] = constrain(servoAngleArr[1] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[1] = constrain(servoAngleArr[1] - angleIncrementVal, 0, 180);
            }
            break;
          case 'C':
            if (packet[i+1] == '0') {
              servoAngleArr[2] = constrain(servoAngleArr[2] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[2] = constrain(servoAngleArr[2] - angleIncrementVal, 0, 180);
            }
            break;
          case 'D':
            if (packet[i+1] == '0') {
              servoAngleArr[3] = constrain(servoAngleArr[3] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[3] = constrain(servoAngleArr[3] - angleIncrementVal, 0, 180);
            }
            break;
          case 'E':
            if (packet[i+1] == '0') {
              servoAngleArr[4] = constrain(servoAngleArr[4] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[4] = constrain(servoAngleArr[4] - angleIncrementVal, 0, 180);
            }
            break;
          case 'F':
            if (packet[i+1] == '0') {
              servoAngleArr[5] = constrain(servoAngleArr[5] + angleIncrementVal, 0, 180);
            } else if (packet[i+1] == '1') {
              servoAngleArr[5] = constrain(servoAngleArr[5] - angleIncrementVal, 0, 180);
            }
            break;
        }
      }

      // Set servo angles
      for (int i = 0; i < 6; i++) {
        setServoAngle(i, servoAngleArr[i]);
      }
    }

    delay(1);
  }

  client.stop();
  Serial.println("Client disconnected");
}