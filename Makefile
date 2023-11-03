
CC=gcc
CFLAGS=-Wall -Wextra
SRC=$(wildcard src/*.c)
OUTPUT=build/8080

default: $(SRC)
    mkdir -p build
    $(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

.PHONY:
    clean

clean:
    rm -rf build
