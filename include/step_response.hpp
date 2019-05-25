#ifndef STEP_RESPONSE_HPP
#define STEP_RESPONSE_HPP

#include <stdint.h>
#include "encoder.hpp"
#include "motor.hpp"

// Configuration structure for step response
typedef struct {
  uint16_t pwm;
  uint16_t sample_time;
  uint16_t nb_measure;
  uint16_t nb_sample;
  uint16_t wait_time;
} config_t;

// Motor measure
typedef struct {
  uint32_t timestamp;
  float position;
  float speed;
} measure_t;

// Execute multiple step response and send the measurement
void step_response(Motor* motor, Encoder* encoder);

#endif
