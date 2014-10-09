/*
 * Arduino Firmware to interface with HC-SR04 Ultrasonic Range Finder
 * For use with Volume Control Python Project
 * AUTHORS:  Charlie Albright
 *           Spencer Kaiser
 */

int trigPin = 12;
int echoPin = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 14400 bits per second:
  Serial.begin(14400);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.write(distance);
  delay(500);
}



