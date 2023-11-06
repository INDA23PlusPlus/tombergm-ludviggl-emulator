
#include "io.h"
#include <stdio.h>

byte_t io_read(byte_t port)
{
    (void)port;
    return 0;
}

void io_write(byte_t port, byte_t data)
{
    (void)port;
    putc(data, stdin);
}
