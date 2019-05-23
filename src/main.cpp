#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"

Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

void setup()
{
  Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop()
{
  read_data(&pwm, sizeof(pwm));
  left_motor.set_pwm(pwm);
}
