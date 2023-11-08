#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mem.h"
#include "tui.h"
#include "cpu.h"

int freerun = 0;

void sighandler(int sig)
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
        printf("Press ENTER to step, or type 'c' to run, 'a' to autostep, 'q' to quit, 'r' to reset, 'mX' to go to address X.\n");
        char c = fgetc(stdin);
        if (c <= 'Z') c += 'a' - 'A';

        if (c == 'q')
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
        else
        {
            cpu_step();
        }
    }

    tui_update();
}
