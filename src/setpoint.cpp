#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(float translation_threshold, float rotation_threshold,
bool orient, bool turn, bool backwards) :
_translation_threshold(translation_threshold), _rotation_threshold(rotation_threshold),
_delta_move({0, 0}), _state(STOP),
_orient(orient), _turn(turn), _backwards(backwards) {}

void Setpoint::set_current_position(const position_t* current_position) {
  _current_position = current_position;
}

void Setpoint::set_setpoint_position(const position_t* setpoint_position) {
  _setpoint_position = setpoint_position;
  if (_orient) {
    _state = ORIENT;
  }
  else {
    _state = MOVE;
  }
}

bool Setpoint::isStopped() const {
  return _state == STOP;
}

delta_move_t* Setpoint::update(bool stop_translation, bool stop_rotation) {
  float delta_x = _setpoint_position->x - _current_position->x;
  float delta_y = _setpoint_position->y - _current_position->y;

  switch(_state) {
    case STOP:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = 0;
      break;
    case ORIENT:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = pi_modulo(atan2f(delta_y, delta_x) - _current_position->theta);

      // Go backwards ?
      if (fabsf(_delta_move.delta_rotation) >= M_PI_2 && _backwards) {
        _delta_move.delta_rotation = pi_modulo(_delta_move.delta_rotation + M_PI);
      }

      if (fabsf(_delta_move.delta_rotation) <= _rotation_threshold) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        if (stop_rotation) {
          _state = MOVE;
        }
      }

      break;
    case MOVE:
      _delta_move.delta_translation = sqrtf(delta_x*delta_x + delta_y*delta_y);
      _delta_move.delta_rotation = pi_modulo(atan2f(delta_y, delta_x) - _current_position->theta);

      // Go backwards ?
      if (fabsf(_delta_move.delta_rotation) >= M_PI_2 && _backwards) {
        _delta_move.delta_translation *= -1;
        _delta_move.delta_rotation = pi_modulo(_delta_move.delta_rotation + M_PI);
      }

      if (fabsf(_delta_move.delta_translation) <= _translation_threshold) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        if (stop_translation) {
          if (_turn) {
            _state = TURN;
          }
          else {
            _state = STOP;
          }
        }
      }

      break;
    case TURN:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = _setpoint_position->theta - _current_position->theta;

      if (fabsf(_delta_move.delta_rotation) <= _rotation_threshold) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        if (stop_rotation) {
          _state = STOP;
        }
      }
      break;
  }

  return &_delta_move;
}
