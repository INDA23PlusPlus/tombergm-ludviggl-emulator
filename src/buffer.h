#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define BUFSIZE 128

typedef struct {
    char data[BUFSIZE];
    size_t front;
} buf_t;

void buf_putc(buf_t *buf, char c);
void buf_print(buf_t *buf);

#endif
