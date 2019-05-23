#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>

typedef struct {
  float left_encoder;
  float right_encoder;
} encoders_t;

encoders_t encoders;
uint16_t threshold = 500;

i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 left_encoder(w);
AS5601 right_encoder(w2);

bool ping;

Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

void setup()
{
  left_encoder.initWire(threshold);
  right_encoder.initWire(threshold);
  Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  read_data(&ping, sizeof(ping));
}

void loop()
{
  // read_data(&pwm, sizeof(pwm));
  // right_motor.set_pwm(pwm);
  // left_motor.set_pwm(pwm);
  // delay(2000);
  // right_motor.set_pwm(0);
  // left_motor.set_pwm(0);

  encoders.left_encoder = left_encoder.read();
  encoders.right_encoder = -right_encoder.read();

  write_data(&encoders, sizeof(encoders));
  delay(10);
}
