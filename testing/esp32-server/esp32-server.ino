#include <secrets.h>
#include <WiFi.h>

#define BUTTON_PIN 32

const char* ssid = "ESP32_Server";
const char* password = "12345678";

WiFiServer server(5000);

void setup() {
  Serial.begin(115200);

  // Create Wi-Fi network
  WiFi.softAP(ssid, password);

  Serial.print("Server IP: ");
  Serial.println(WiFi.softAPIP());

  // Start TCP server
  server.begin();
  Serial.println("TCP server started");

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  WiFiClient client = server.available();

  // Wait for client to connect
  while (!client) {
    client = server.available();
    delay(100);
  }

  Serial.println("Client connected");

  while (client.connected()) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      client.println("button");
    }
    else {
      client.println("no button");
    }

    delay(200);
  }

  client.stop();
  Serial.println("Client disconnected");
}