#ifndef RAMP_HPP
#define RAMP_HPP

// Calculate the position of the robot
class Ramp {
public:
  // Initialize Ramp class
  Ramp(float max_speed, float max_acceleration, float sample_time);

  // Return true if stopped
  bool isStopped();

  // Compute speed ramp
  void compute(float* delta_position);

private:
  float _filter_limit(float value, float limit);

  float _max_speed;
  float _max_acceleration;
  float _sample_time;
  float _speed;
};

#endif
