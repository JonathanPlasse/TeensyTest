#include "step_response.hpp"
#include "binary_serial.hpp"

void step_response(Motor* motor, Encoder* encoder) {
  // Initialization of the structure for communication
  config_t config;
  measure_t measure = {0, 0, 0};

  uint16_t nb_measure_done = 0;
  uint16_t nb_sample_done = 0;

  uint32_t last_time;
  float last_position;

  read_data(&config, sizeof(config));
  write_data(&config, sizeof(config));

  last_time = millis();

  motor->set_pwm(config.pwm);

  // Handle the step response
  while (nb_measure_done < config.nb_measure) {
    measure.timestamp = millis();

    if (measure.timestamp - last_time > config.sample_time) {
      last_time += config.sample_time;

      // Get the measurement
      measure.timestamp = millis();
      last_position = measure.position;
      measure.position = encoder->read();
      measure.speed = (float)(measure.position-last_position)
        / config.sample_time * 1000;

      // Send data
      if (nb_sample_done < config.nb_sample) {
        write_data(&measure, sizeof(measure));
        nb_sample_done++;
      }
      // Stop motor and wait
      else if (nb_sample_done < config.nb_sample + config.wait_time / config.sample_time) {
        motor->set_pwm(0);
        nb_sample_done++;
      }
      // Power the motor for another step response
      else if (nb_measure_done + 1 < config.nb_measure) {
        nb_measure_done++;
        nb_sample_done = 0;
        motor->set_pwm(config.pwm);
      }
    }
  }
}
