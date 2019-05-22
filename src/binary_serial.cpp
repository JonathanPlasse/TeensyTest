#include "binary_serial.hpp"

void read_data(const void* data, const size_t nb_bytes) {
  while (Serial.available() < nb_bytes);
  Serial.readBytes((byte*) data, nb_bytes);
}

void write_data(const void* data, const size_t nb_bytes) {
  byte* byte_data = (byte*) data;
  Serial.write(byte_data, nb_bytes);
}
