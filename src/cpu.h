#ifndef CPU_H
#define CPU_H

#include "definitions.h"

typedef struct {

    union {
        byte_t byte_reg[8];

        struct {
            byte_t b;
            byte_t c;
            byte_t d;
            byte_t e;
            byte_t h;
            byte_t l;
            byte_t __padding;
            byte_t a;
        };
    };

    byte_t f;

    addr_t pc;
    addr_t sp;

    byte_t inte;
    byte_t halt;

} cpu_t;

extern cpu_t cpu;

void cpu_step(void);

#endif
