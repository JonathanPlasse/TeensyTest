#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>
#include "encoder.hpp"
#include "step_response.hpp"

// Motors
Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

// Encoders
typedef struct {
  float left_encoder;
  float right_encoder;
} encoders_t;

encoders_t encoders;

uint16_t threshold = 1023;

float center_distance_motor = 144, center_distance_encoder = 241;


i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 left_as(w), right_as(w2);
Encoder left_encoder(&left_as, &right_as, center_distance_motor, center_distance_encoder);
Encoder right_encoder(&right_as, &left_as, center_distance_motor, center_distance_encoder);

bool ping;

uint32_t time;

void setup() {
  left_as.initWire(threshold, 1);
  right_as.initWire(threshold, -1);

  Serial.begin(9600); // USB is always 12 Mbit/sec

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // typedef struct {
  //   uint16_t p1;
  //   uint16_t p2;
  //   uint16_t p3;
  //   uint16_t p4;
  //   uint16_t p5;
  // } ping_t;
  // ping_t ping;
  //
  // read_data(&ping, sizeof(ping));
  // write_data(&ping, sizeof(ping));

  // step_response(&left_motor, &left_encoder);
  step_response(&right_motor, &right_encoder);

  // read_data(&ping, sizeof(ping));
  // read_data(&pwm, sizeof(pwm));
  //
  // time = millis();
}

void loop() {
  // if (millis() - time < 1000) {
  //   // right_motor.set_pwm(pwm);
  //   left_motor.set_pwm(pwm);
  //
  //   encoders.left_encoder = left_encoder.read();
  //   encoders.right_encoder = right_encoder.read();
  //
  //   write_data(&encoders, sizeof(encoders));
  //   delay(10);
  // }
  // else {
  //   // right_motor.set_pwm(0);
  //   left_motor.set_pwm(0);
  //
  //   read_data(&ping, sizeof(ping));
  //   read_data(&pwm, sizeof(pwm));
  //
  //   time = millis();
  // }
}
