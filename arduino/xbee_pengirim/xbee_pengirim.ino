#include <SoftwareSerial.h>

// HC-SR04
const int trigPin = 8;
const int echoPin = 9;

// XBee
SoftwareSerial XBeeSerial(10, 11);

// MQTT topic (DIKIRIM dari node)
const char* topic = "unpar/ged9/b1/a2";

void setup() {
  Serial.begin(9600);
  XBeeSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  // Kirim: <topic|data>
  XBeeSerial.print('<');
  XBeeSerial.print(topic);
  XBeeSerial.print('|');
  XBeeSerial.print(distance);
  XBeeSerial.println('>');

  Serial.print("Sent: ");
  Serial.print(topic);
  Serial.print(" = ");
  Serial.println(distance);

  delay(1000);
}
