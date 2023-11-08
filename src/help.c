
#include "help.h"
#include <stdio.h>

void showhelp(void)
{
    printf("\033[2J");
    printf(
            "---------------------------------------------------\n"
            "*************** Intel 8080 Emulator ***************\n"
            "---------------------------------------------------\n"
            "\n"
          );
    printf(
            "Commands:\n"
            "            <enter>            Step\n"
            "            n  <enter>         Step over\n"
            "            c  <enter>         Run\n"
            "            r  <enter>         Reset\n"
            "            a  <enter>         Auto-step\n"
            "            mX <enter>         Show memory address X\n"
            "            q  <enter>         Quit\n"
            "            h  <enter>         Show this help message\n"
            "            ^C                 Stop / quit\n"
            "\n"
            "Press <enter> to close this message.\n"
          );
    fgetc(stdin);
    fgetc(stdin);
}
