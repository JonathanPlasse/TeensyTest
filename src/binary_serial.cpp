#include "binary_serial.hpp"

void read_data(const void* data, const size_t nb_bytes) {
  while (Serial.available() < nb_bytes);
  Serial.readBytes((char*) data, nb_bytes);
}

void write_data(const void* data, const size_t nb_bytes) {
  char* byte_data = (char*) data;
  Serial.write(byte_data, nb_bytes);
}

bool read_data_if(const void* data, const size_t nb_bytes) {
  if (Serial.available() < nb_bytes) {
    return false;
  }
  Serial.readBytes((char*) data, nb_bytes);
  return true;
}
