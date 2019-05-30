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
const uint8_t order = 1;

float left_r[order+1] = {0.026693494674808037, -0.02185478499738868};
float left_s[order+1] = {1.0, -1.0};
float left_t[order+1] = {0.026693494674808037, -0.02185478499738868};
float right_r[order+1] = {0.027583277830634974, -0.022583277830634973};
float right_s[order+1] = {1.0, -1.0};
float right_t[order+1] = {0.027583277830634974, -0.022583277830634973};

float min_command = -200, max_command = 200;

// Initialization of the system variables
control_t left_control = {1000, 0, 0};
control_t right_control = {1000, 0, 0};

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

// Initialization of Setpoint
uint8_t i_position = 0;

// square move
// const uint8_t nb_move = 4;
// position_t setpoint_position[nb_move] = {{10, 0, 0}, {10, 10, 0}, {0, 10, 0}, {0, 0, 0}};
// Go back and forth
const uint8_t nb_move = 2;
position_t setpoint_position[nb_move] = {{5, 0, M_PI_2}, {0, 0, 0}};

delta_move_t* delta_move;
float step_threshold = 100;
Setpoint setpoint(step_threshold, true, false, true);

Ramp translation_ramp(20, 20, sample_time/1000.);
Ramp rotation_ramp(2, 2, sample_time/1000.);

float translation_speed;
float rotation_speed;

uint32_t start_time, stop_time;

const float command_scale = 20;

bool ping;

void setup() {
  left_as.initWire(threshold, 1);
  right_as.initWire(threshold, -1);

  // Set rst coefficient
  left_rst.set_rst(left_r, left_s, left_t, order);
  right_rst.set_rst(right_r, right_s, right_t, order);

  Serial.begin(9600); // USB is always 12 Mbit/sec

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // step_response(&left_motor, &left_encoder);
  // step_response(&right_motor, &right_encoder);

  // Set position pointer to Setpoint
  setpoint.set_current_position(odometry.get_position());
  setpoint.set_setpoint_position(&setpoint_position[i_position]);

  read_data(&ping, sizeof(ping));
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
  // if (setpoint.isStopped()) {// && translation_ramp.isStopped() && rotation_ramp.isStopped()) {
  //   i_position = (i_position+1)%nb_move;
  //   setpoint.set_setpoint_position(&setpoint_position[i_position]);
  // }

  // Update setpoint
  delta_move = setpoint.update();

  translation_speed = translation_ramp.compute(delta_move->delta_translation);
  rotation_speed = rotation_ramp.compute(delta_move->delta_rotation);

  // Update reference
  left_control.reference = cm2step_motor(translation_speed) - rad2step_motor(rotation_speed);
  right_control.reference = cm2step_motor(translation_speed) + rad2step_motor(rotation_speed);

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
