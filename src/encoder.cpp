#include "encoder.hpp"

Encoder::Encoder(AS5601* near_encoder, AS5601* far_encoder,
float center_distance_motor, float center_distance_encoder) :
_near_encoder(near_encoder), _far_encoder(far_encoder),
_center_distance_motor(center_distance_motor),
_center_distance_encoder(center_distance_encoder) {}

float Encoder::read() {
  return _near_encoder->read()/2*(1+_center_distance_motor/_center_distance_encoder)
       + _far_encoder->read()/2*(1-_center_distance_motor/_center_distance_encoder);
}
