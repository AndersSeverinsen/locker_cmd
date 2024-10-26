#include <ESP32Servo.h>

static const int LED_BLUE = 5;
static const int LED_RED = 27;

static const int servoPin = 13;

static const int solenoidPin = 21;

Servo servo1;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  servo1.attach(servoPin);
  pinMode(solenoidPin, OUTPUT);
}

void open() {
  // Open
  digitalWrite(LED_BLUE, HIGH); 
  digitalWrite(LED_RED, LOW);
  servo1.write(20);
  delay(1000);
  digitalWrite(solenoidPin, HIGH);
}

void closed() {
  // Closed
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, HIGH); 
  servo1.write(110);
  delay(1000);
  digitalWrite(solenoidPin, LOW);
}

void loop() {
  open();
  delay(1000);
  closed();
  delay(1000);

}
