#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include <stdint.h>
#include "position.hpp"

// Calculate the position of the robot
class Odometry {
public:
  // Initialize Odometry class
  Odometry();

  // Update odometry
  void update(float left_step, float right_step);

  // Get position
  const position_t* get_position() const;

  // Set position
  void set_position(const position_t* position);

private:
  // Position
  position_t _position;
  float _last_left_step;
  float _last_right_step;
};

#endif
