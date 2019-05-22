#include <Arduino.h>
#include "binary_serial.hpp"

// int c = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  Serial.println("Test");
  delay(1000);
}
