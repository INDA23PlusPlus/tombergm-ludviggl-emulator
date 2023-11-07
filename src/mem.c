#include "mem.h"
#include "tui.h"

byte_t mem[ADDRSPACE_SIZE];

byte_t mem_read(addr_t addr)
{
    //tui_addr = addr;
    return mem[addr];
}

void mem_write(addr_t addr, byte_t data)
{
    tui_addr = addr;
    mem[addr] = data;
}
