#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdint.h>

// Execute function every sample_time
void timer(uint32_t time, uint8_t sample_time);

// Control the motor
void control_system();

// Read position from serial
bool read_position();

#endif
