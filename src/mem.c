
#include "mem.h"

byte_t mem[ADDRSPACE_SIZE];

byte_t mem_read(addr_t addr)
{
    return mem[addr];
}

void mem_write(addr_t addr, byte_t data)
{
    mem[addr] = data;
}
