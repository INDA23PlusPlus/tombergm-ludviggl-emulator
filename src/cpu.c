
#include "cpu.h"
#include "definitions.h"
#include "mem.h"
#include "assert.h"

cpu_t cpu;

byte_t get_code()
{
    const byte_t code = mem[cpu.pc];
    cpu.pc++;
    return code;
}

byte_t *reg_ptr(byte_t op)
{
    switch (op)
    {
        case OP_A: return &cpu.a;
        case OP_F: return &cpu.f;
        case OP_B: return &cpu.b;
        case OP_C: return &cpu.c;
        case OP_D: return &cpu.d;
        case OP_E: return &cpu.e;
        case OP_H: return &cpu.h;
        case OP_L: return &cpu.l;
        default:   assert(0 && "undefined register");
    }
}

void push8(byte_t data)
{
    mem[cpu.sp--] = data;
}

byte_t pop8()
{
    return mem[cpu.sp++];
}

void push16(addr_t data)
{
    push8(GETH(data));
    push8(GETL(data));
}

addr_t pop16()
{
    addr_t data = 0;
    data = SETL(data, pop8());
    data = SETH(data, pop8());
    return data;
}

// returns what A register should be
byte_t setflags(int result)
{
    byte_t flags = 0;
    flags &= !!result        << FLAG_Z;
    flags &= (result < 0)    << FLAG_S;
    flags &= (result & 1)    << FLAG_P;
    flags &= (result >= 256) << FLAG_CY;
    flags &= (result < 0)    << FLAG_CY; // borrow
    // TODO: FLAG_AC
    cpu.f = flags;
    return (byte_t)result;
}

void op_write(byte_t op, byte_t data)
{
    switch (op)
    {
        case OP_A: cpu.a = data; break;
        case OP_F: cpu.f = data; break;
        case OP_B: cpu.b = data; break;
        case OP_C: cpu.c = data; break;
        case OP_D: cpu.d = data; break;
        case OP_E: cpu.e = data; break;
        case OP_H: cpu.h = data; break;
        case OP_L: cpu.l = data; break;
        case OP_MEM: mem_write(cpu.hl, data); break;
        default: assert(0 && "undefined operand");
    }
}

byte_t op_read(byte_t op)
{
    switch (op)
    {
        case OP_A: return cpu.a;
        case OP_F: return cpu.f;
        case OP_B: return cpu.b;
        case OP_C: return cpu.c;
        case OP_D: return cpu.d;
        case OP_E: return cpu.e;
        case OP_H: return cpu.h;
        case OP_L: return cpu.l;
        case OP_MEM: return mem_read(cpu.hl);
        default: assert(0 && "undefined operand"); return 0;
    }
}

void cpu_exec()
{
    const byte_t code = get_code();
    int r;

    switch (code)
    {
    }

    switch (code & LOW_OP_MASK)
    {
        // Add
        case INSTRBITS_ADD:
            r = (int)cpu.a + (int)op_read(OP_2(code));
            cpu.a = setflags(r);
            break;
        // Add with carry
        case INSTRBITS_ADC:
            r = (int)cpu.a + (int)op_read(OP_2(code)) + (int)GETBIT(cpu.f, FLAG_CY);
            cpu.a = setflags(r);
            break;
        // Subtract
        case INSTRBITS_SUB:
            r = (int)cpu.a - (int)op_read(OP_2(code));
            cpu.a = setflags(r);
            break;
        // Subtract with borrow
        case INSTRBITS_SBB:
            r = (int)cpu.a - (int)op_read(OP_2(code)) - (int)GETBIT(cpu.f, FLAG_CY);
            cpu.a = setflags(r);
            break;
        // AND
        case INSTRBITS_ANA:
            cpu.a &= op_read(OP_2(code));
            cpu.f = UNSETBIT(cpu.f, BIT(FLAG_CY));
            break;
        // OR
        case INSTRBITS_ORA:
            cpu.a |= op_read(OP_2(code));
            cpu.f = UNSETBIT(cpu.f, BIT(FLAG_CY));
            cpu.f = UNSETBIT(cpu.f, BIT(FLAG_AC));
            break;
        // XOR
        case INSTRBITS_XRA:
            cpu.a ^= op_read(OP_2(code));
            cpu.f = UNSETBIT(cpu.f, BIT(FLAG_CY));
            cpu.f = UNSETBIT(cpu.f, BIT(FLAG_AC));
            break;
        // Compare
        case INSTRBITS_CMP:
            r = (int)cpu.a - (int)op_read(OP_2(code));
            setflags(r);
            break;
    }

    switch (code & HIGH_OP_MASK)
    {
        // Increment
        case INSTRBITS_INR:
            (*reg_ptr(OP_1(code)))++;
            break;
        // Decrement
        case INSTRBITS_DCR:
            (*reg_ptr(OP_1(code)))--;
            break;
        // Reset
        case INSTRBITS_RST:
            push16(cpu.hl);
            cpu.pc = 8 * (addr_t)OP_1(code);
            break;
    }
}
