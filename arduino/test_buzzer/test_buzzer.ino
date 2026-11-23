// Test: active buzzer
// Beeps twice every 2 seconds (like an alert).
// Wiring: + (long leg) -> D12, - -> GND

const int BUZZER_PIN = 12;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

// Makes a short beep of the given length in milliseconds
void beep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  beep(150);
  delay(150);
  beep(150);
  delay(2000);
}