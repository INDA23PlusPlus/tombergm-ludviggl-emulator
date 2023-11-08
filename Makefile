CC=gcc
ASM8080=asm8080
CFLAGS=-Wall -Wextra
SRC=$(wildcard src/*.c)
TEST_SRC=test/fib.S test/helloworld.S
TEST_ASM=$(TEST_SRC:test/%.S=build/%.asm)
TEST_BIN=$(TEST_SRC:test/%.S=build/%.bin)
OUTPUT=build/8080

.PHONY: default test clean

default: $(OUTPUT) $(TEST_BIN)

test: $(TEST_BIN)

clean:
	rm -rf build

$(OUTPUT): $(SRC) | build/
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_ASM): build/%.asm: test/%.S | build/
	$(CPP) -P $< -o $@

$(TEST_BIN): build/%.bin: build/%.asm | build/
	$(ASM8080) $< -o$(@:%.bin=%)

build/:
	mkdir -p $@
