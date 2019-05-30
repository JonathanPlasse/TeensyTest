#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>
#include <math.h>

// Center distance between the two wheel
const float center_distance_motor = 14.4;
const float center_distance_encoder = 24.1;

// Count per revolution of encoder
const uint32_t resolution_motor = 1633;
const uint32_t resolution_encoder = 4096;

// Wheel diameter
const float wheel_perimeter = 4.5*M_PI;

// Wheel diameter ratio (left/right)
const float wheel_ratio = 1;

// Position structure
typedef struct {
  float x;
  float y;
  float theta;
} position_t;

// Return an angle between [-pi, pi)
float pi_modulo(float angle);

// Convert step to cm
float step2cm(float step);

// Convert cm to step
float cm2step(float cm);

// Convert step to rad
float step2rad(float step);

// Convert rad to step
float rad2step(float rad);

// Convert step motor to step encoder
float step_motor2encoder(float motor);

// Convert step encoder to step motor
float step_encoder2motor(float encoder);

#endif
