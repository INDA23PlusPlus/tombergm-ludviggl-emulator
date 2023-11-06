
#ifndef SYSTEM_H
#define SYSTEM_H

#include "definitions.h"

typedef struct {

    union {
        struct {
            byte_t a;
            byte_t f;
        };
        addr_t af;
    };

    union {
        struct {
            byte_t b;
            byte_t c;
        };
        addr_t bc;
    };

    union {
        struct {
            byte_t d;
            byte_t e;
        };
        addr_t de
 ;
    };

    union {
        struct {
            byte_t h;
            byte_t l;
        };
        addr_t hl
    };

    addr_t pc;
    addr_t sp;

} cpu_t;

extern cpu_t cpu;

void cpu_exec();

#endif
