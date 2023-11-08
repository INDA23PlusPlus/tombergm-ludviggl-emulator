CC=gcc
ASM8080=asm8080
CFLAGS=-Wall -Wextra
SRC=$(wildcard src/*.c)
TEST_SRC=test/fib.s test/helloworld.s
TEST_BIN=$(TEST_SRC:test/%.s=build/%.bin)
OUTPUT=build/8080

.PHONY: default test clean

default: $(OUTPUT) $(TEST_BIN)

test: $(TEST_BIN)

clean:
	rm -rf build

$(OUTPUT): $(SRC) | build/
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_BIN): build/%.bin: test/%.s | build/
	$(ASM8080) $< -o$(@:%.bin=%)

build/:
	mkdir -p $@
