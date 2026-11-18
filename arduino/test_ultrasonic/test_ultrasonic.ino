//Ultrasonic sensor
long distance;
long duration;
#define trigPin 7
#define echoPin 8

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  // Send a 10 microsecond pulse to start a measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure how long (in microseconds) the echo takes to come back
   duration = pulseIn(echoPin, HIGH);

  // Convert time to distance:
  // sound travels 0.034 cm per microsecond, divide by 2 because it goes and comes back
  distance = duration * 0.0340 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}

