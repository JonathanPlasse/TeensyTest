#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>

typedef struct {
  uint16_t left_encoder;
  uint16_t right_encoder;
  int16_t left_nb_revolution;
  int16_t right_nb_revolution;
} encoders_t;

encoders_t encoders = {0, 0, 0, 0}, last_encoders;

i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 as(w);
AS5601 as2(w2);

bool ping;

Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

void setup()
{
  w->begin(I2C_MASTER,0x00,I2C_PINS_7_8, I2C_PULLUP_EXT, 100000);
  w2->begin(I2C_MASTER,0x00,I2C_PINS_3_4, I2C_PULLUP_EXT, 100000);
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

  last_encoders = encoders;

  encoders.left_encoder = as.getRawAngle();
  encoders.right_encoder = as2.getRawAngle();

  if ()

  write_data(&encoders, sizeof(encoders));
  delay(10);
}
