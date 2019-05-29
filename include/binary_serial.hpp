#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <Arduino.h>

// Wait until data is read
void read_data(const void* data, const size_t nb_bytes);

// Write data
void write_data(const void* data, const size_t nb_bytes);

// Read data if available
bool read_data_if(const void* data, const size_t nb_bytes);

#endif
