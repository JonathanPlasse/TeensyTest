#include <Arduino.h>

void setup()
{
  Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(5000);
  digitalWrite(13, LOW);
}

void loop()
{
  Serial.println("Hello World...");
  delay(1000);  // do not print too fast!
}
