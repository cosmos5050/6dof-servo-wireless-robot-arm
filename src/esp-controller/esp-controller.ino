#include <WiFi.h>

// Joystick pins
#define JOY_1_RX_PIN 34
#define JOY_1_RY_PIN 35
#define JOY_2_RX_PIN 36
#define JOY_2_RY_PIN 39

const char* ssid = "ESP32_Server";
const char* password = "12345678";

const char* serverIP = "192.168.4.1";
const int serverPort = 5000;

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Connect to server ESP32's Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi");

  Serial.print("Client IP: ");
  Serial.println(WiFi.localIP());

  // Connect to TCP server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
  }
  else {
    Serial.println("TCP connection failed");
  }
}

void loop() {

  if (client.connected()) {

    // Read joystick
    int x1 = analogRead(JOY_1_RX_PIN);

    // Convert joystick position to angle
    int joyAngle = map(x1, 0, 4095, 0, 180);

    // Determine command
    if (joyAngle > 100) {
      client.println("right");
    }
    else if (joyAngle < 80) {
      client.println("left");
    }

    Serial.print("Joystick angle: ");
    Serial.println(joyAngle);
  }

  delay(100);
}