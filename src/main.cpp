#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>
#include <Encoder.h>
#include "step_response.hpp"
#include "rst.hpp"
#include "control.hpp"
#include "main.hpp"
#include "odometry.hpp"
#include "setpoint.hpp"
#include "ramp.hpp"

// Motors
Motor left_motor(31, 30), right_motor(28, 29);
int16_t pwm;

// Encoders
uint16_t threshold = 1023;

i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 left_as(w), right_as(w2);
Encoder left_encoder(20, 21);
Encoder right_encoder(22, 23);
float left_position, right_position;
float last_left_position, last_right_position;

// Initialization of the RST setting
const uint8_t order = 2;

float left_r[order+1] = {0.044489157791346726, -0.03642464166231447, 0.};
float left_s[order+1] = {1., -1., 0.};
float left_t[order+1] = {0.13346747337404016, -0.19825224056963686, 0.07284928332462894};
float right_r[order+1] = {0.04597212971772495, -0.03763879638439162, 0.};
float right_s[order+1] = {1., -1., 0.};
float right_t[order+1] = {0.13791638915317483, -0.20486064858862474, 0.07527759276878324};

float min_command = -255, max_command = 255;

// Initialization of the system variables
control_t left_control = {0, 0, 0};
control_t right_control = {0, 0, 0};

float error_threshold = 0;
float pwm_threshold = 0;

// Initialization of the RST
Rst left_rst(&left_control, min_command, max_command,
         error_threshold, pwm_threshold);
Rst right_rst(&right_control, min_command, max_command,
         error_threshold, pwm_threshold);

// Initialization for the timer
uint8_t sample_time = 5;
uint32_t time, last_time;

// Initialization of Odometry
Odometry odometry;
position_t init_position;

// Initialization of Setpoint
uint8_t i_position = 0;

// square move
// const uint8_t nb_move = 4;
// position_t setpoint_position[nb_move] = {{10, 0, 0}, {10, 10, 0}, {0, 10, 0}, {0, 0, 0}};
// Go back and forth
const uint8_t nb_move = 2;
position_t setpoint_position[nb_move] = {{20, 0, -M_PI_2}, {0, 0, 0}};

delta_move_t* delta_move;
float translation_threshold = 0.2;
float rotation_threshold = 0.02;
Setpoint setpoint(translation_threshold, rotation_threshold, true, false, true);

Ramp translation_ramp(200, 200, sample_time/1000.);
Ramp rotation_ramp(5, 5, sample_time/1000.);

float translation_speed;
float rotation_speed;

uint32_t start_time, stop_time;

const float speed_scale = 5;

bool ping;

void setup() {
  left_as.initWire(threshold, 1);
  right_as.initWire(threshold, -1);

  // Set rst coefficient
  left_rst.set_rst(left_r, left_s, left_t, order);
  right_rst.set_rst(right_r, right_s, right_t, order);

  Serial.begin(9600); // USB is always 12 Mbit/sec

  pinMode(13, OUTPUT);

  // step_response(&left_motor, &left_encoder);
  // step_response(&right_motor, &right_encoder);

  while (!read_data_if(&init_position, sizeof(init_position))) {
    if (millis() % 200 < 100) {
      digitalWrite(13, HIGH);
    }
    else {
      digitalWrite(13, LOW);
    }
  }
  digitalWrite(13, HIGH);

  odometry.set_position(&init_position);

  // Set position pointer to Setpoint
  setpoint.set_current_position(odometry.get_position());
  setpoint.set_setpoint_position(&setpoint_position[i_position]);

  // start_time = stop_time = millis();
}

void loop() {
  // Execute timer
  timer(millis(), sample_time);
}

void timer(uint32_t time, uint8_t sample_time) {
  static uint32_t last_time = millis();

  if (time - last_time > sample_time) {
    // Update last_time
    last_time += sample_time;
    control_system();
  }
}

void control_system() {
  // Read motor position
  last_left_position = left_position;
  last_right_position = right_position;
  left_position = left_encoder.read();
  right_position = right_encoder.read();
  // Update speed measure
  left_control.measurement = (left_position-last_left_position)/sample_time*1000;
  right_control.measurement = (right_position-last_right_position)/sample_time*1000;

  // Odometry
  odometry.update(left_as.read(), right_as.read());

  // // Update goal point
  if (setpoint.isStopped()) {
    i_position = (i_position+1)%nb_move;
    setpoint.set_setpoint_position(&setpoint_position[i_position]);
  }

  // Update setpoint
  delta_move = setpoint.update(translation_ramp.isStopped(), rotation_ramp.isStopped());

  translation_speed = translation_ramp.compute(delta_move->delta_translation);
  rotation_speed = rotation_ramp.compute(delta_move->delta_rotation);

  // Update reference
  left_control.reference = cm2step_motor(translation_speed)/speed_scale - rad2step_motor(rotation_speed)/speed_scale;
  right_control.reference = cm2step_motor(translation_speed)/speed_scale + rad2step_motor(rotation_speed)/speed_scale;

  // Compute control command
  left_rst.compute();
  right_rst.compute();

  // Apply the command on the motors
  left_motor.set_pwm(left_control.command);
  right_motor.set_pwm(right_control.command);

  // Debug
  static uint8_t c = 10;
  if (c++ == 10) {
    // write_data(&right_control.reference, sizeof(right_control.reference));
    // write_data(&right_control.measurement, sizeof(right_control.measurement));
    // write_data(&left_control.reference, sizeof(left_control.reference));
    // write_data(&left_control.measurement, sizeof(left_control.measurement));
    write_data(odometry.get_position(), sizeof(position_t));
    c = 0;
  }
}
