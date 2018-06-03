void setup() {
  pinMode(10, OUTPUT);
}

void loop() {
  for (int j = 0; j < 3; ++j) {
    for (int i = 0; i < 50; ++i) {
      digitalWrite(10, HIGH);
      delayMicroseconds(i*(j+1));
      digitalWrite(10, LOW);
      delayMicroseconds(i*(j+1));
    }
    delay(200);
  }
  delay(1000);
}
