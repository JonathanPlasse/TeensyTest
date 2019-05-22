#include <Arduino.h>
#include "binary_serial.hpp"

uint32_t c;

void setup()
{
  Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  read_data(&c, sizeof(c));
  write_data(&c, sizeof(c));
  digitalWrite(13, LOW);
}

void loop()
{
  write_data(&c, sizeof(c));
  c++;
  delay(10);
}
