#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>

// Motors
Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

// Encoders
typedef struct {
  float left_encoder;
  float right_encoder;
} encoders_t;

encoders_t encoders;

typedef struct {
  float left_motor;
  float right_motor;
} motors_t;

motors_t motors;

uint16_t threshold = 1023;

float entraxe_motor = 144.6, entraxe_encoder = 240.9;


i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 left_encoder(w), right_encoder(w2);

bool ping;

uint32_t time;

void setup()
{
  left_encoder.initWire(threshold);
  right_encoder.initWire(threshold);

  Serial.begin(9600); // USB is always 12 Mbit/sec

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  read_data(&ping, sizeof(ping));
  read_data(&pwm, sizeof(pwm));

  time = millis();
}

void loop()
{
  if (millis() - time < 2000) {
    right_motor.set_pwm(pwm);
    left_motor.set_pwm(pwm);

    encoders.left_encoder = left_encoder.read();
    encoders.right_encoder = -right_encoder.read();

    motors.left_motor = encoders.left_encoder/2*(1+entraxe_motor/entraxe_encoder)
                      + encoders.right_encoder/2*(1-entraxe_motor/entraxe_encoder);
    motors.right_motor = encoders.left_encoder/2*(1-entraxe_motor/entraxe_encoder)
                       + encoders.right_encoder/2*(1+entraxe_motor/entraxe_encoder);

    write_data(&encoders, sizeof(encoders));
    write_data(&motors, sizeof(motors));
    delay(10);
  }
  else {
    right_motor.set_pwm(0);
    left_motor.set_pwm(0);

    read_data(&ping, sizeof(ping));
    read_data(&pwm, sizeof(pwm));

    time = millis();
  }
}
