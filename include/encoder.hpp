#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <AS5601.h>

class Encoder {
public:
  // Initialisation of the Encoder class
  Encoder(AS5601* near_encoder, AS5601* far_encoder,
          float center_distance_motor, float center_distance_encoder);

  // Read motor position
  float read();

private:
  AS5601* _near_encoder;
  AS5601* _far_encoder;
  const float _center_distance_motor;
  const float _center_distance_encoder;
};

#endif
