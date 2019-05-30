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

float step_encoder2mm(float step_encoder) {
  return step_encoder * wheel_perimeter / resolution_encoder;
}

float mm2step_encoder(float mm) {
  return mm / wheel_perimeter * resolution_encoder;
}

float step_encoder2rad(float step_encoder) {
  return step_encoder * wheel_perimeter / resolution_encoder / center_distance_encoder * 2;
}

float rad2step_encoder(float rad) {
  return rad / wheel_perimeter * resolution_encoder * center_distance_encoder / 2;
}

float step_motor2mm(float step_motor) {
  return step_motor * wheel_perimeter / resolution_motor;
}

float mm2step_motor(float mm) {
  return mm / wheel_perimeter * resolution_motor;
}

float step_motor2rad(float step_motor) {
  return step_motor * wheel_perimeter / resolution_motor / center_distance_motor * 2;
}

float rad2step_motor(float rad) {
  return rad / wheel_perimeter * resolution_motor * center_distance_motor / 2;
}
