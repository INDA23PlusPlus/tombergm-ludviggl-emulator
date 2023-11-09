
# Intel 8080 emulator

Run with `./build/8080 program.bin`.
Programs can be assembled with [begoon/asm8080](https://github.com/begoon/asm8080).

Characters are printed by writing to port 0. Writing to port 1 stops the emulators free running mode,
and thus acts like a breakpoint.
