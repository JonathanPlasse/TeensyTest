#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dir_pin, uint8_t pwm_pin) :
_dir_pin(dir_pin), _pwm_pin(pwm_pin) , _pwm(0) {
  pinMode(_dir_pin, OUTPUT);
  analogWrite(_pwm_pin, 0);
}

int16_t Motor::get_pwm() const {
  return (int16_t)_pwm;
}

void Motor::set_pwm(int16_t pwm) {
  if (pwm > 0) {
    digitalWrite(_dir_pin, LOW);
    analogWrite(_pwm_pin, pwm);
  }
  else {
    digitalWrite(_dir_pin, HIGH);
    analogWrite(_pwm_pin, -pwm);
  }
  _pwm = (float)pwm;
}
