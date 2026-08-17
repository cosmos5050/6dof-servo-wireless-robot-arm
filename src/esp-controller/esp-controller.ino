#include <WiFi.h>

// Joystick pins
#define JOY_1_RX_PIN 34
#define JOY_1_RY_PIN 35
#define JOY_2_RX_PIN 36
#define JOY_2_RY_PIN 39
// Button pins
#define BUTTON_OPEN_PIN 26
#define BUTTON_CLOSE_PIN 14

const char* ssid = "ESP32_Server";
// Pass has to be >8 chars long for some reason
const char* password = "12345678";

const char* serverIP = "192.168.4.1";
const int serverPort = 5000;

// Recorded joystick adc centre vals
const int joy1RxCentreMax = 3105;
const int joy1RxCentreMin = 3019;
const int joy1RyCentreMax = 3017;
const int joy1RyCentreMin = 2898;
const int joy2RxCentreMax = 2973;
const int joy2RxCentreMin = 2939;
const int joy2RyCentreMax = 3049;
const int joy2RyCentreMin = 3007;
// Val for deadband around recorded cetre vals
const int deadBand = 20;

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

  pinMode(BUTTON_OPEN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_CLOSE_PIN, INPUT_PULLUP);
}

void loop() {
  // Packet for transmittal
  // Format AxBxCxDxEx
  String toTransmit = "";
  
  if (client.connected()) {
    // Read joystick adc val
    int x1 = analogRead(JOY_1_RX_PIN);
    int y1 = analogRead(JOY_1_RY_PIN);
    int x2 = analogRead(JOY_2_RX_PIN);
    int y2 = analogRead(JOY_2_RY_PIN);

    // Determine joy stick commands for motor
    // Joy1 Rx
    toTransmit += "A";
    if (x1 > joy1RxCentreMax + deadBand) {
      toTransmit += "0";
    }
    else if (x1 < joy1RxCentreMin - deadBand) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }
    // Joy1 Ry
    toTransmit += "B";
    if (y1 > joy1RyCentreMax + deadBand) {
      toTransmit += "0";
    }
    else if (y1 < joy1RyCentreMin - deadBand) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }
    // Joy2 Rx
    toTransmit += "C";
    if (x2 > joy2RxCentreMax + deadBand) {
      toTransmit += "0";
    }
    else if (x2 < joy2RxCentreMin - deadBand) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }
    // Joy2 Ry
    toTransmit += "D";
    if (y2 > joy2RyCentreMax + deadBand) {
      toTransmit += "0";
    }
    else if (y2 < joy2RyCentreMin - deadBand) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }
    // Buttons
    toTransmit += "E";
    if (digitalRead(BUTTON_OPEN_PIN) == LOW) {
      toTransmit += "0";
    } else if (digitalRead(BUTTON_CLOSE_PIN) == LOW) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }

    Serial.println(toTransmit);
    // Transmit
    client.println(toTransmit);
  }

  delay(100);
}