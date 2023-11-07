
#include "tui.h"
#include "io.h"
#include "cpu.h"
#include "definitions.h"
#include "disas.h"
#include "mem.h"
#include "buffer.h"
#include <string.h>
#include <stdio.h>

addr_t tui_addr;

#define MEM_ROWS 8
#define HL "\x1b[2;47;30m"
#define RST "\x1b[1;0m"

#define REG(x) (prev.x != cpu.x ? HL : ""), cpu.x
#define FLAG(x) (GETBIT(cpu.f, x) ? HL "1" RST : "0")

void hexdump(addr_t);

void tui_update(void)
{
    static cpu_t prev = { 0 };

    printf("\033[2J");

    io_dump();

    printf("\n------------------------------------------------\n");

    printf(
            "Registers:\n"
            "             S   Z   AC   P   CY\n"
            "A:    %s0x%02x" RST " \u2502 %s" RST " | %s" RST " | %s" RST "  | %s" RST " | %s" RST "\n"
            "B:    %s0x%02x" RST " \u2502 C:    %s0x%02x" RST "\n"
            "D:    %s0x%02x" RST " \u2502 E:    %s0x%02x" RST "\n"
            "H:    %s0x%02x" RST " \u2502 L:    %s0x%02x" RST "\n"
            "SP: %s0x%04x"   RST " \u2502 PC: %s0x%04x"   RST "\n"
            "\n"
            ,
            REG(a), FLAG(FLAG_S), FLAG(FLAG_Z), FLAG(FLAG_AC), FLAG(FLAG_P), FLAG(FLAG_CY),
            REG(b), REG(c),
            REG(d), REG(e),
            REG(h), REG(l),
            REG(sp), REG(pc)
    );

    prev = cpu;

    printf("Last write:\n");
    hexdump(tui_addr);

    printf("\nDisassembly:\n");
    addr_t disas_addr = cpu.pc;
    for (int i = 0; i < 5; i++)
    {
        int n = 0;
        char disas_buf[32];
        addr_t next_addr = disas(disas_buf, disas_addr);

        printf("0x%04x:   ", disas_addr);

        if (i == 0)
        {
            printf(HL);
        }

        while (disas_addr != next_addr)
        {
            if (n++ != 0)
            {
                printf(" ");
            }
            printf("%02x", mem[disas_addr++]);
        }

        while (n < 4)
        {
            printf("   ");
            n++;
        }

       printf("%s" RST "\n", disas_buf);
    }

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

        if (addr == at || addr == cpu.pc || addr == cpu.sp)
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
