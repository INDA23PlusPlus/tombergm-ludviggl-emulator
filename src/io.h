#ifndef IO_H
#define IO_H

#include "definitions.h"

byte_t io_read(byte_t port);
void io_write(byte_t port, byte_t data);

#endif
