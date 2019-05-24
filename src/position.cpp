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
  return step * wheel_perimeter / resolution;
}

float cm2step(float cm) {
  return cm / wheel_perimeter * resolution;
}

float step2rad(float step) {
  return step * wheel_perimeter / resolution / center_distance * 2;
}

float rad2step(float rad) {
  return rad / wheel_perimeter * resolution * center_distance / 2;
}
