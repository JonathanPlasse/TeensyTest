#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <Arduino.h>

void read_data(const void* data, const size_t nb_bytes);
void write_data(const void* data, const size_t nb_bytes);

#endif
