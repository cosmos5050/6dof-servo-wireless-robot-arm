
#define JOY_1_RX 34
#define JOY_1_RY 35
#define JOY_2_RX 36
#define JOY_2_RY 39

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
