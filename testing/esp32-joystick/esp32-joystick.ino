
#define JOY_1_RX_PIN 34
#define JOY_1_RY_PIN 35
#define JOY_2_RX_PIN 36
#define JOY_2_RY_PIN 39

void setup() {
  Serial.begin(9600);
}

void loop() {
  int x1 = analogRead(JOY_1_RX);
  int y1 = analogRead(JOY_1_RY);
  int x2 = analogRead(JOY_2_RX);
  int y2 = analogRead(JOY_2_RY);

  Serial.print("Joystick 1\tX: ");
  Serial.print(x1);
  Serial.print("\tY: ");
  Serial.println(y1);
  Serial.print("Joystick 2\tX: ");
  Serial.print(x2);
  Serial.print("\tY: ");
  Serial.println(y2);

  delay(100);
}
