/*
 * Arduino Firmware to interface with HC-SR04 Ultrasonic Range Finder
 * For use with Volume Control Python Project
 * AUTHORS:  Charlie Albright
 *           Spencer Kaiser
 */

const int buttonPin = 2;
const int ledPin = 8;
const int trigPin = 12;
const int echoPin = 13;
const int buttonSig = 999;

int buttonState;
int lastButtonState = LOW;




void setup() {
  Serial.begin(14400);
  
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  
  int reading = digitalRead(buttonPin);
  
  if (reading == HIGH && lastButtonState == LOW) {
    Serial.println(buttonSig);
    lastButtonState = HIGH;
  } else if (reading == LOW && lastButtonState == HIGH) {
    lastButtonState = LOW;
  } else if (reading == LOW && lastButtonState == LOW) {
    long duration, distance;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    if (distance < 50) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    Serial.println(distance);
  } else {
    Serial.println(100);
  }
  
  delay(500);
}



