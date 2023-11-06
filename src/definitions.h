
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>

typedef uint8_t byte_t ;
typedef uint16_t addr_t;

#define ADDRSPACE_SIZE (1<<16)

// Operands
#define OP_A   0b111
#define OP_F   0b1000
#define OP_B   0b000
#define OP_C   0b001
#define OP_D   0b010
#define OP_E   0b011
#define OP_H   0b100
#define OP_L   0b101
#define OP_MEM 0b110

typedef enum
{
    FLAG_Z,
    FLAG_S,
    FLAG_P,
    FLAG_CY,
    FLAG_AC
} flag_t;

#define BIT(b) (1 << (b))
#define GETBIT(x, b) (((x) & BIT(b)) >> b)
#define SETBIT(x, b) ((x) | BIT(b))
#define UNSETBIT(x, b) ((x) & ~BIT(b))

// Extract the first operand of an instruction
#define OP_1(x) (((x) >> 3) & 7)

// Extract the second operand of an instruction
#define OP_2(x) ((x) & 7)

// Get/set low/high bytes of 16-bit word
#define SETH(x, b) (((x) & 0xff) | ((b) << 8))
#define SETL(x, b) (((x) & 0xff00) | (b))
#define GETH(x) (((x) >> 8) & 0xff)
#define GETL(x) ((x) & 0xff)

// Mask out operand from instruction
#define LOW_OP_MASK  0b11111000
#define HIGH_OP_MASK 0b11000111
#define BOTH_OP_MASK 0b11000000

// Masked with LOW_OP_MASK
#define INSTRBITS_ADD 0b10000000
#define INSTRBITS_ADC 0b10001000
#define INSTRBITS_SUB 0b10010000
#define INSTRBITS_SBB 0b10011000
#define INSTRBITS_ANA 0b10100000
#define INSTRBITS_XRA 0b10101000
#define INSTRBITS_ORA 0b10110000
#define INSTRBITS_CMP 0b10111000

// Masked with HIGH_OP_MASK
#define INSTRBITS_MVI 0b00000110
#define INSTRBITS_INR 0b00000100
#define INSTRBITS_DCR 0b00000101
#define INSTRBITS_RST 0b11000111

// Masked with BOTH_OP_MASK
#define INSTRBITS_MOV 0b01000000

// Unambigous instructions
enum instr
{
#define MACRO(i, c) INSTR_##i = c,
#include "instr_raw.h"
#undef MACRO
};

#endif
