#ifndef MEM_H
#define MEM_H

#include "definitions.h"

extern byte_t mem[ADDRSPACE_SIZE];

byte_t mem_read(addr_t addr);
void mem_write(addr_t addr, byte_t data);

#endif
