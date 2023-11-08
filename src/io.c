#include <stdio.h>
#include "buffer.h"
#include "io.h"

extern int break_on_nl;
extern int freerun;

static buf_t buf = { 0 };

byte_t io_read(byte_t port)
{
    (void)port;
    return 0;
}

void io_write(byte_t port, byte_t data)
{
    (void)port;
    buf_putc(&buf, data);

    if (break_on_nl && data == '\n')
    {
        freerun = 0;
    }
}

void io_dump()
{
    buf_print(&buf);
}
