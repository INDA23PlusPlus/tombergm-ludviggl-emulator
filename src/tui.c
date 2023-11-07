
#include "tui.h"
#include "cpu.h"
#include "definitions.h"
#include "mem.h"
#include <string.h>
#include <stdio.h>

addr_t tui_addr;

#define MEM_ROWS 8
#define HL "\x1b[2;47;30m"
#define RST "\x1b[1;0m"

#define REG(x) (prev.x != cpu.x ? HL : ""), cpu.x

void hexdump(addr_t);

void tui_update(void)
{
    static cpu_t prev = { 0 };

    printf("\033[2J");

    printf(
            "Registers:\n"
            "             S   Z   AC   P   CY\n"
            "A:    %s0x%02x" RST " \u2502 %s%d" RST " | %s%d" RST " | %s%d" RST "  | %s%d" RST " | %s%d" RST "\n"
            "B:    %s0x%02x" RST " \u2502 C:    %s0x%02x" RST "\n"
            "D:    %s0x%02x" RST " \u2502 E:    %s0x%02x" RST "\n"
            "H:    %s0x%02x" RST " \u2502 L:    %s0x%02x" RST "\n"
            "SP: %s0x%04x"   RST " \u2502 PC: %s0x%04x"   RST "\n"
            "\n"
            ,
            REG(a), REG(s), REG(z), REG(ac), REG(p), REG(cy),
            REG(b), REG(c),
            REG(d), REG(e),
            REG(h), REG(l),
            REG(sp), REG(pc)
    );

    prev = cpu;

    int start = tui_addr & 0xfff0;
    start -= 8 * MEM_ROWS;
    if (start < 0) start = 0;
    if (start >= ADDRSPACE_SIZE) start = ADDRSPACE_SIZE - 16 * MEM_ROWS;

    printf("Last write:\n");
    hexdump(tui_addr);

    //printf("Program counter:\n");
    //hexdump(cpu.pc);

    printf("\n\n");
}

void hexdump(addr_t at)
{
    int start = at & 0xfff0;
    start -= 8 * MEM_ROWS;
    if (start < 0) start = 0;
    if (start >= ADDRSPACE_SIZE) start = ADDRSPACE_SIZE - 16 * MEM_ROWS;
    printf("           0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");

    for (addr_t addr = start; addr < start + 16 * MEM_ROWS; addr++)
    {
        if (addr % 16 == 0)
        {
            if (addr != start) printf("\n");
            printf("0x%04x:    ", addr);
        }

        if (addr == at)
        {
            printf(HL "%02x" RST " ", mem[addr]);
        }
        else
        {
            printf("%02x ", mem[addr]);
        }
    }
    printf("\n");
}
