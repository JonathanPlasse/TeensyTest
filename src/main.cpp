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

float left_r[order+1] = {1.8827428713360554, -3.5398220258579505, 1.6615039332829569};
float left_s[order+1] = {1.0, -0.951458020924246, -0.048541979075753916};
float left_t[order+1] = {0.046497044003430535, -0.042072265242368484, 0.};
float right_r[order+1] = {1.8499995170519505, -3.478259903669117, 1.6326082127041228};
float right_s[order+1] = {1.0, -0.951458020924246, -0.048541979075753916};
float right_t[order+1] = {0.04568839975989261, -0.04134057367293599, 0.};

float min_command = -70, max_command = 70;

// Initialization of the system variables
control_t left_control = {7000, 0, 0};
control_t right_control = {7000, 0, 0};

float error_threshold = 50;
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
