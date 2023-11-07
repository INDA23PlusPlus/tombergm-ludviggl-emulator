#ifndef SYSTEM_H
#define SYSTEM_H

#include "definitions.h"

typedef struct {

    union {
        byte_t byte_reg[8];
        addr_t addr_reg[4];

        struct {
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
                addr_t de;
            };

            union {
                struct {
                    byte_t h;
                    byte_t l;
                };
                addr_t hl;
            };

            union {
                struct {
                    byte_t a;
                    union
                    {
                        byte_t f;
                        struct
                        {
                            byte_t s  : 1;
                            byte_t z  : 1;
                            byte_t    : 1;
                            byte_t ac : 1;
                            byte_t    : 1;
                            byte_t p  : 1;
                            byte_t    : 1;
                            byte_t cy : 1;
                        };
                    };
                };
                addr_t af;
            };
        };
    };

    addr_t pc;
    addr_t sp;

    byte_t inte;
    byte_t halt;

} cpu_t;

extern cpu_t cpu;

void cpu_step(void);

#endif
