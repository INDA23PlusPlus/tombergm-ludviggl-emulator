#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem.h"
#include "tui.h"
#include "cpu.h"

int main(int argc, char *argv[])
{

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

        printf("Press ENTER to step, or type 'c' to run, 'a' to autostep, 'q' to quit.\n");

        char c = fgetc(stdin);

        if (c == 'q')
        {
            break;
        }
        else if (c == 'c')
        {
            while (!cpu.halt)
            {
                cpu_step();
            }
        }
        else if (c == 'a')
        {
            for (;;)
            {
                cpu_step();

                if (cpu.halt)
                {
                    break;
                }
                else
                {
                    tui_update();
                    usleep(20000);
                }
            }
        }
        else
        {
            cpu_step();
        }
    }

    tui_update();
}
