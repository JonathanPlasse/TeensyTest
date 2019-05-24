#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

// Count per revolution of encoder
const uint32_t resolution = 1633;

// Center distance between the two wheel
const float center_distance = 19.65;

// Wheel diameter
const float wheel_perimeter = 23.4;

// Wheel diameter ratio (left/right)
const float wheel_ratio = 1.003;

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

#endif
