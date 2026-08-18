#include <WiFi.h>

// Joystick pins
#define JOY_1_RX_PIN 34
#define JOY_1_RY_PIN 35
#define JOY_2_RX_PIN 36
#define JOY_2_RY_PIN 39
// Button pins
#define BUTTON_GRIP_ROT_L_PIN 25
#define BUTTON_GRIP_ROT_R_PIN 26
#define BUTTON_GRIP_CLOSE_PIN 14
#define BUTTON_GRIP_OPEN_PIN 33

const char* ssid = "ESP32_Server";
// Pass has to be >8 chars long for some reason
const char* password = "12345678";

const char* serverIP = "192.168.4.1";
const int serverPort = 5000;

// Recorded joystick adc centre vals
const int joy1RxCentreMax = 1657;
const int joy1RxCentreMin = 1634;
const int joy1RyCentreMax = 1693;
const int joy1RyCentreMin = 1677;
const int joy2RxCentreMax = 1627;
const int joy2RxCentreMin = 1615;
const int joy2RyCentreMax = 1654;
const int joy2RyCentreMin = 1637;
// Val for deadband around recorded centre vals
const int deadBand = 200;

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

  pinMode(BUTTON_GRIP_ROT_L_PIN, INPUT);
  pinMode(BUTTON_GRIP_ROT_R_PIN, INPUT);
  pinMode(BUTTON_GRIP_CLOSE_PIN, INPUT);
  pinMode(BUTTON_GRIP_OPEN_PIN, INPUT);
}

void loop() {
  // Packet for transmittal
  // Format AxBxCxDxEx
  // A-D: joystick axes
  // E: grip rotation, 0 = left, 1 = right, 2 = neutral
  // F: grip, 0 = close, 1 = open, 2 = neutral
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
    if (digitalRead(BUTTON_GRIP_ROT_L_PIN) == LOW) {
      toTransmit += "0";
    } else if (digitalRead(BUTTON_GRIP_ROT_R_PIN) == LOW) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }
    toTransmit += "F";
    if (digitalRead(BUTTON_GRIP_CLOSE_PIN) == LOW) {
      toTransmit += "0";
    } else if (digitalRead(BUTTON_GRIP_OPEN_PIN) == LOW) {
      toTransmit += "1";
    } else {
      toTransmit += "2";
    }

    // Debug
    Serial.print("X1: ");
    Serial.print(x1);

    Serial.print(" Y1: ");
    Serial.print(y1);

    Serial.print(" X2: ");
    Serial.print(x2);

    Serial.print(" Y2: ");
    Serial.print(y2);

    Serial.print(" | Packet: ");
    Serial.println(toTransmit);
    
    // Transmit
    client.println(toTransmit);
  }

  delay(20);
}