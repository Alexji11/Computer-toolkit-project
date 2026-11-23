// Test: push button (Pomodoro start/pause)
// Prints a message every time the button is pressed.
// Wiring: one side -> D4, other side -> GND (uses the internal pull-up resistor)

const int BUTTON_PIN = 4;
bool lastState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // HIGH = not pressed, LOW = pressed
  Serial.begin(9600);
}

void loop() {
  bool currentState = digitalRead(BUTTON_PIN);

  // Only react at the moment the button goes from "not pressed" to "pressed"
  if (lastState == HIGH && currentState == LOW) {
    Serial.println("Button pressed! (Pomodoro start/pause)");
  }

  lastState = currentState;
  delay(50);  // small pause to ignore bouncing of the button
}