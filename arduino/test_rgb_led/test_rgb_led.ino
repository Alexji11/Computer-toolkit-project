
const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

// Sets the colour of the LED (each value from 0 to 255)
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void loop() {
  setColor(0, 255, 0);    // green  = good comfort
  delay(1000);
  setColor(255, 120, 0);  // yellow = medium comfort
  delay(1000);
  setColor(255, 0, 0);    // red    = bad comfort
  delay(1000);
}

