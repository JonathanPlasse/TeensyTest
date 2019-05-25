#ifndef CONTROL_HPP
#define CONTROL_HPP

// Control structure
typedef struct {
  float reference;
  float measurement;
  float command;
} control_t;

typedef struct {
  float delta_translation;
  float delta_rotation;
} delta_move_t;

#endif
