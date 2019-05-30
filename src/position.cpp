#include "position.hpp"
#include <math.h>

float pi_modulo(float angle)
{
    angle = fmodf(angle, 2 * M_PI);
    if (angle > M_PI) {
        return angle - 2 * M_PI;
    }
    if (angle < -M_PI) {
        return angle + 2 * M_PI;
    }
    return angle;
}

float step2cm(float step) {
  return step * wheel_perimeter / resolution_encoder;
}

float cm2step(float cm) {
  return cm / wheel_perimeter * resolution_encoder;
}

float step2rad(float step) {
  return step * wheel_perimeter / resolution_encoder / center_distance_motor * 2;
}

float rad2step(float rad) {
  return rad / wheel_perimeter * resolution_encoder * center_distance_motor / 2;
}

float step_motor2encoder(float motor) {
  return motor / resolution_motor * resolution_encoder
    / center_distance_motor * center_distance_encoder;
}

float step_encoder2motor(float encoder) {
  return encoder / resolution_encoder * resolution_motor
    / center_distance_encoder * center_distance_motor;
}
