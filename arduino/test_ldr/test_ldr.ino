// LDR sensor
#define LDR_PIN  A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int rawValue = analogRead(LDR_PIN);        // 0 - 1023 (10-bit ADC)

  float voltage = rawValue * 5.0 / 1023.0;   // convert to volts (0 - 5 V)
  int lightPercent = map(rawValue, 0, 1023, 0, 100);  // convert to 0 - 100 %

  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print("  Voltage: ");
  Serial.print(voltage);
  Serial.print(" V  Light: ");
  Serial.print(lightPercent);
  Serial.println(" %");

  delay(500);
}