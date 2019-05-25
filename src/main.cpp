#include <Arduino.h>
#include "binary_serial.hpp"
#include "motor.hpp"
#include <i2c_t3.h>
#include <AS5601.h>
#include "encoder.hpp"
#include "step_response.hpp"
#include "rst.hpp"
#include "control.hpp"
#include "main.hpp"

// Motors
Motor left_motor(28, 29), right_motor(31, 30);
int16_t pwm;

uint16_t threshold = 1023;

float center_distance_motor = 144, center_distance_encoder = 241;

i2c_t3* w = &Wire;
i2c_t3* w2 = &Wire2;
AS5601 left_as(w), right_as(w2);
Encoder left_encoder(&left_as, &right_as, center_distance_motor, center_distance_encoder);
Encoder right_encoder(&right_as, &left_as, center_distance_motor, center_distance_encoder);

// Initialization of the RST setting
const uint8_t order = 2;

float left_r[order+1] = {11.982290563982566, -21.150421835929734, 9.327423307345397};
float left_s[order+1] = {1.0, -0.7274929189476966, -0.2725070810523034};
float left_t[order+1] = {1.673893584123461, -1.514601548725231, 0.};
float right_r[order+1] = {11.773902902000263, -20.782588412696175, 9.165207249826347};
float right_s[order+1] = {1.0, -0.7274929189476966, -0.27250708105230337};
float right_t[order+1] = {1.6447823913560964, -1.4882606522256616, 0.};

float min_command = -30, max_command = 30;

// Initialization of the system variables
control_t left_control = {5000, 0, 0};
control_t right_control = {5000, 0, 0};

float error_threshold = 100;
float pwm_threshold = 40;

// Initialization of the RST
Rst left_rst(&left_control, min_command, max_command,
         error_threshold, pwm_threshold);
Rst right_rst(&right_control, min_command, max_command,
         error_threshold, pwm_threshold);

// Initialization for the timer
uint8_t sample_time = 5;
uint32_t time, last_time;

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

  read_data(&ping, sizeof(ping));
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
  left_control.measurement = left_encoder.read();
  right_control.measurement = right_encoder.read();

  // Debug
  static uint8_t c = 10;
  if (c++ == 10) {
    write_data(&right_control.command, sizeof(right_control.command));
    write_data(&right_control.measurement, sizeof(right_control.measurement));
    write_data(&left_control.command, sizeof(left_control.command));
    write_data(&left_control.measurement, sizeof(left_control.measurement));
    c = 0;
  }

  // Compute control command
  left_rst.compute();
  right_rst.compute();

  // Apply the command on the motors
  left_motor.set_pwm(left_control.command);
  right_motor.set_pwm(right_control.command);
}
