#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>
#include <math.h>

// Center distance between the two wheel
const float center_distance_motor = 14.4;
const float center_distance_encoder = 24;

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

// Convert step encoder to cm
float step_encoder2cm(float step_encoder);

// Convert cm to step encoder
float cm2step_encoder(float cm);

// Convert step encoder to rad
float step_encoder2rad(float step_encoder);

// Convert rad to step encoder
float rad2step_encoder(float rad);

// Convert step motor to cm
float step_motor2cm(float step_motor);

// Convert cm to step motor
float cm2step_motor(float cm);

// Convert step motor to rad
float step_motor2rad(float step_motor);

// Convert rad to step motor
float rad2step_motor(float rad);

#endif
