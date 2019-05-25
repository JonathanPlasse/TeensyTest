#include "rst.hpp"
#include "math.h"

Rst::Rst(control_t* control,
         float min_command, float max_command,
         float error_threshold, float pwm_threshold) :
_control(control),
_min_command(min_command), _max_command(max_command),
_error_threshold(error_threshold), _pwm_threshold(pwm_threshold) {
  reset();
}

void Rst::set_rst(float* r, float* s, float* t, uint8_t order) {
  _r = r;
  _s = s;
  _t = t;
  _order = order;
  reset();
}

void Rst::reset() {
  for (int i = 0 ; i <= MAX_RST_ORDER ; i++) {
    _reference_hist[i] = 0;
    _measurement_hist[i] = 0;
    _command_hist[i] = 0;
  }
}
void Rst::compute() {
  // Update history
  for (int i = 0 ; i < _order ; i++) {
    _reference_hist[_order-i] = _reference_hist[_order-i-1];
    _measurement_hist[_order-i] = _measurement_hist[_order-i-1];
    _command_hist[_order-i] = _command_hist[_order-i-1];
  }
  _reference_hist[0] = _control->reference;
  _measurement_hist[0] = _control->measurement;

  // Compute partial command signal
  _command_hist[0] = 0;
  for (int i = 0 ; i <= _order ; i++) {
    _command_hist[0] += _t[i]*_reference_hist[i]
                      - _r[i]*_measurement_hist[i];
  }

  // Control signal dead zone before integration to avoid limit cycle
  if ((fabsf(_reference_hist[0] - _measurement_hist[0]) < _error_threshold)
   && (fabsf(_command_hist[0]) < _pwm_threshold)
   && (_reference_hist[0] == _reference_hist[1])) {
    _command_hist[0] = 0;
  }

  // Compute final command signal
  for (int i = 1 ; i <= _order ; i++) {
    _command_hist[0] -= _s[i]*_command_hist[i];
  }

  // Sature command signal
  if (_command_hist[0] > _max_command)
    _command_hist[0] = _max_command;
  else if (_command_hist[0] < _min_command)
    _command_hist[0] = _min_command;

  // Update command signal
  _control->command = _command_hist[0];
}
