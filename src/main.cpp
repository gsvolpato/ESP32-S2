#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("ESP32-S2 Hello World Program Started");
}

void loop() {
  Serial.println("Hello World");
  delay(2000);
}