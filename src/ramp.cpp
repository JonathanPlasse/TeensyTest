#include "ramp.hpp"
#include "math.h"

Ramp::Ramp(float max_speed, float max_acceleration, float sample_time) : _max_speed(max_speed),
_max_acceleration(max_acceleration), _sample_time(sample_time), _speed(0) {}

void Ramp::compute(float* delta_position) {

  float breaking_distance = _speed * _speed / 2 / _max_acceleration;
  float delta_position_sign = copysignf(1., *delta_position);
  float acceleration = 0;

  // Compute acceleration
  if (delta_position_sign == copysignf(1., _speed)) {
    if (fabsf(*delta_position) <= breaking_distance ||
        fabsf(*delta_position) <= _max_acceleration * _sample_time * _sample_time / 2) {
      acceleration = -_filter_limit(_speed / _sample_time, _max_acceleration);
    }
    else if (fabsf(_speed) >= _max_speed) {
      // maximal velocity reached -> just cruise
      acceleration = 0;
    }
    else {
      // we can go faster
      acceleration = delta_position_sign * _max_acceleration;
    }
  }
  else {
    // driving away from target position -> turn around
    if (fabsf(*delta_position) <= _max_acceleration * _sample_time * _sample_time / 2) {
      acceleration = -_filter_limit(_speed / _sample_time, _max_acceleration);
    }
    else {
      acceleration = delta_position_sign * _max_acceleration;
    }
  }

  // Compute speed
  _speed += acceleration * _sample_time;

  // Compute delta_position
  *delta_position = _speed * _sample_time;
}

bool Ramp::isStopped() {
  // Flag: possible problem of equality of a float to 0
  return _speed == 0;
}

float Ramp::_filter_limit(float value, float limit) {
  if (value > limit) {
    return limit;
  }
  else if (value < -limit) {
    return -limit;
  }
  else {
    return value;
  }
}
