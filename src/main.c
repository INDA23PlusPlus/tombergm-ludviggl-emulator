#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "cpu.h"
#include "disas.h"
#include "help.h"
#include "mem.h"
#include "tui.h"

static int freerun = 0;

static void sighandler(int sig)
{
    (void)sig;
    if (freerun)
    {
        freerun = 0;
    }
    else
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{

    signal(SIGINT, sighandler);

    if (argc == 1)
    {
        printf("ERROR: No input file provided\n");
        exit(1);
    }

    const char *fname = argv[1];

    FILE *f = fopen(fname, "rb");

    if (f == NULL)
    {
        printf("ERROR: Could not open file %s\n", fname);
        exit(1);
    }

    const size_t fsize = fread(mem, sizeof(mem), 1, f);
    (void)fsize;

    for (;;)
    {
        tui_update();
        printf("Press <h> <enter> to show a list of commands.\n");
        char c = fgetc(stdin);
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';

        if (c == 'n')
        {
            addr_t bp = disas(NULL, cpu.pc);
            freerun = 1;
            while (!cpu.halt && freerun && cpu.pc != bp)
            {
                cpu_step();
            }
            freerun = 0;
        }
        else if (c == 'q')
        {
            break;
        }
        else if (c == 'c')
        {
            freerun = 1;
            while (!cpu.halt && freerun)
            {
                cpu_step();
            }
            freerun = 0;
        }
        else if (c == 'm')
        {
            char buf[32];
            int i = 0;
            while ((c = fgetc(stdin)))
            {
                if (c == '\n') break;
                buf[i++] = c;
            }
            buf[i] = '\0';
            tui_addr = strtol(buf, NULL, 16);
        }
        else if (c == 'a')
        {
            freerun = 1;
            while (freerun)
            {
                cpu_step();

                if (cpu.halt || !freerun)
                {
                    freerun = 0;
                    break;
                }
                else
                {
                    tui_update();
                    usleep(20000);
                }
            }
        }
        else if (c == 'r')
        {
            cpu = (cpu_t) { 0 };
        }
        else if (c == 'h')
        {
            showhelp();
        }
        else
        {
            cpu_step();
        }

        while (c != '\n')
        {
            c = fgetc(stdin);
        }
    }

    tui_update();
}
