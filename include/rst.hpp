#ifndef RTS_HPP
#define RTS_HPP

#include <stdint.h>
#include "control.hpp"

#define MAX_RST_ORDER 4

class Rst {
public:
  // Initialize Rst class.
  Rst(control_t* control,
      float min_command, float max_command,
      float error_threshold, float pwm_threshold);

  // Set rst polynomials coefficient.
  void set_rst(float* r, float* s, float* t, uint8_t order);

  // Initialize rst history.
  void reset();

  // Compute command.
  void compute();

private:
  // System variable
  control_t* _control;

  // Control signal interval
  float _min_command;
  float _max_command;

  // Thresholds
  float _error_threshold;
  float _pwm_threshold;

  // System variable history
  float _reference_hist[MAX_RST_ORDER+1];
  float _measurement_hist[MAX_RST_ORDER+1];
  float _command_hist[MAX_RST_ORDER+1];

  // Order of the rst controller
  uint8_t _order;

  // Coefficient of the rst
  float* _r;
  float* _s;
  float* _t;
};

#endif
