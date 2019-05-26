#ifndef SETPOINT_HPP
#define SETPOINT_HPP

#include "position.hpp"
#include "control.hpp"

typedef enum {STOP, ORIENT, MOVE, TURN} State;

class Setpoint {
public:
  // Initialize Setpoint class
  Setpoint(float step_threshold, bool orient, bool turn, bool backwards);

  // Set current_position pointer
  void set_current_position(const position_t* current_position);

  // Set setpoint_position pointer
  void set_setpoint_position(const position_t* setpoint_position);

  // Return true if stopped
  bool isStopped() const;

  // Update reference
  delta_move_t* update();

private:
  float _step_threshold;
  bool _orient, _turn, _backwards;
  const position_t* _current_position;
  const position_t* _setpoint_position;
  delta_move_t _delta_move;
  State _state;
};

#endif
