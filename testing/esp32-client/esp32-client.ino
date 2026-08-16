#include <secrets.h>
#include <WiFi.h>

#define LED_PIN 33

const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* serverIP = "192.168.4.1";
const int serverPort = 5000;

WiFiClient client;

void setup() {
  Serial.begin(115200);

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

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (client.available()) {
    String message = client.readStringUntil('\n');
    message.trim();

    Serial.print("Received: ");
    Serial.println(message);

    if (message == "button") {
      digitalWrite(LED_PIN, HIGH);
    }
    else if (message == "no button") {
      digitalWrite(LED_PIN, LOW);
    }
  }

  delay(100);
}