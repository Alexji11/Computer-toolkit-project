// =====================================================================
// StudyBuddy – Smart Study Desk Assistant
// ---------------------------------------------------------------------
// Reads:  DHT11 (temperature + humidity), LDR (light), HC-SR04 (distance)
// Shows:  comfort level on an RGB LED (green / yellow / red)
// Alerts: buzzer beeps when the comfort becomes bad
// Input:  push button (Pomodoro start/pause)
// Sends:  all data to Node-RED over the serial port (USB)
//
// Serial message format (Arduino -> Node-RED), one line every second:
//   T:<temperature °C>;H:<humidity %>;L:<light raw 0-1023>;D:<distance cm>
//   example: T:23.5;H:58.0;L:450;D:35
// Button event (sent when the button is pressed):
//   BTN:PRESSED
// =====================================================================

#include <DHT.h>

// ---------- Pins ----------
const int DHT_PIN    = 2;
const int BUTTON_PIN = 4;
const int TRIG_PIN   = 7;
const int ECHO_PIN   = 8;
const int RED_PIN    = 9;
const int GREEN_PIN  = 10;
const int BLUE_PIN   = 11;
const int BUZZER_PIN = 12;
const int LDR_PIN    = A0;

// ---------- Comfort thresholds (later adjustable from the dashboard) ----------
float tempMin  = 20.0;   // °C
float tempMax  = 24.0;   // °C
float humMin   = 40.0;   // %
float humMax   = 60.0;   // %
int   lightMin = 600;    // raw value 0-1023, below this it is too dark

// ---------- Timing: millis() instead of delay() ----------
const unsigned long SEND_INTERVAL = 1000;  // send data every 1 second
const unsigned long DHT_INTERVAL  = 2000;  // the DHT11 can only measure every 2 seconds
const unsigned long DEBOUNCE_TIME = 50;    // ignore button bouncing for 50 ms

unsigned long lastSendTime     = 0;
unsigned long lastDhtTime      = 0;
unsigned long lastButtonChange = 0;
unsigned long buzzerOffTime    = 0;        // 0 = buzzer is off

// ---------- Sensor values ----------
DHT dht(DHT_PIN, DHT11);
float temperature = NAN;   // NAN = "no valid value yet"
float humidity    = NAN;
int   lightRaw    = 0;
int   distanceCm  = 0;

// ---------- Other state ----------
bool lastButtonState  = HIGH;  // HIGH = not pressed (INPUT_PULLUP)
int  lastComfortLevel = 0;     // 0 = good, 1 = medium, 2 = bad


// =====================================================================
// setup(): runs once when the Arduino starts
// =====================================================================
void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();
}


// =====================================================================
// loop(): runs again and again, never blocks (no long delay())
// =====================================================================
void loop() {
  unsigned long now = millis();

  // Every 2 seconds: read the DHT11
  if (now - lastDhtTime >= DHT_INTERVAL) {
    lastDhtTime = now;
    readDHT();
  }

  // Every second: read light + distance, update the LED and send the data
  if (now - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = now;
    lightRaw   = analogRead(LDR_PIN);
    distanceCm = readDistance();
    updateComfortLed();
    sendData();
  }

  // As often as possible: check the button and switch the buzzer off in time
  checkButton();
  updateBuzzer(now);
}


// =====================================================================
// Sensors
// =====================================================================

// Reads temperature and humidity. Keeps the old values if the reading fails.
void readDHT() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    temperature = t;
    humidity = h;
  }
}

// Measures the distance with the ultrasonic sensor, in cm.
// Returns 0 if no echo came back (Node-RED will filter that out).
int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout of 30 ms (about 5 m) so the program never hangs waiting for an echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // Sound travels 0.034 cm per microsecond; divide by 2 because it goes and comes back
  return duration * 0.034 / 2;
}


// =====================================================================
// Comfort level + RGB LED
// =====================================================================

// Counts how many values are outside their comfort range.
// 0 problems = good, 1 problem = medium, 2 or more = bad.
int getComfortLevel() {
  int problems = 0;

  if (temperature < tempMin || temperature > tempMax) problems++;
  if (humidity < humMin || humidity > humMax)         problems++;
  if (lightRaw < lightMin)                             problems++;

  if (problems == 0) return 0;
  if (problems == 1) return 1;
  return 2;
}

// Sets the colour of the RGB LED (each value 0-255)
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

// Shows the comfort level on the LED and beeps when it becomes bad
void updateComfortLed() {
  int level = getComfortLevel();

  if (level == 0)      setColor(0, 255, 0);    // green
  else if (level == 1) setColor(255, 120, 0);  // yellow
  else                 setColor(255, 0, 0);    // red

  // Beep only at the moment the comfort changes to bad (not every second)
  if (level == 2 && lastComfortLevel != 2) {
    beep(300);
  }
  lastComfortLevel = level;
}


// =====================================================================
// Button
// =====================================================================

// Sends "BTN:PRESSED" at the moment the button is pressed
void checkButton() {
  bool state = digitalRead(BUTTON_PIN);

  if (state != lastButtonState && millis() - lastButtonChange > DEBOUNCE_TIME) {
    lastButtonChange = millis();
    lastButtonState = state;

    if (state == LOW) {          // LOW = pressed
      Serial.println("BTN:PRESSED");
      beep(50);                  // short click sound as feedback
    }
  }
}


// =====================================================================
// Buzzer (non-blocking)
// =====================================================================

// Switches the buzzer on; updateBuzzer() switches it off after "duration" ms
void beep(unsigned long duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerOffTime = millis() + duration;
}

// Switches the buzzer off when its time is over
void updateBuzzer(unsigned long now) {
  if (buzzerOffTime != 0 && now >= buzzerOffTime) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerOffTime = 0;
  }
}


// =====================================================================
// Serial communication
// =====================================================================

// Sends one line with all sensor values
void sendData() {
  Serial.print("T:");
  Serial.print(temperature, 1);   // 1 decimal
  Serial.print(";H:");
  Serial.print(humidity, 1);
  Serial.print(";L:");
  Serial.print(lightRaw);
  Serial.print(";D:");
  Serial.println(distanceCm);
}