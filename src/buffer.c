
#include "buffer.h"
#include <stdio.h>

void buf_putc(buf_t *buf, char c)
{
    buf->data[buf->front] = c;
    buf->front = (buf->front + 1) % BUFSIZE;
}

void buf_print(buf_t *buf)
{
    printf("%.*s", BUFSIZE - (int)buf->front, &buf->data[buf->front]);
    printf("%.*s", (int)buf->front, buf->data);
}
