#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>
#include <math.h>

// Center distance between the two wheel in mm
const float center_distance_motor = 144;
const float center_distance_encoder = 240;

// Count per revolution of encoder
const uint32_t resolution_motor = 1633;
const uint32_t resolution_encoder = 4096;

// Wheel diameter in mm
const float wheel_perimeter = 45*M_PI;

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

// Convert step encoder to mm
float step_encoder2mm(float step_encoder);

// Convert mm to step encoder
float mm2step_encoder(float mm);

// Convert step encoder to rad
float step_encoder2rad(float step_encoder);

// Convert rad to step encoder
float rad2step_encoder(float rad);

// Convert step motor to mm
float step_motor2mm(float step_motor);

// Convert mm to step motor
float mm2step_motor(float mm);

// Convert step motor to rad
float step_motor2rad(float step_motor);

// Convert rad to step motor
float rad2step_motor(float rad);

#endif
