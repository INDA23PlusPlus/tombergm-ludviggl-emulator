#include "assert.h"
#include "cpu.h"
#include "definitions.h"
#include "io.h"
#include "mem.h"

#define BYTE_REG(r) (cpu.byte_reg[( (r) < OP_MEM ? (r) : (r - 1))])
#define ADDR_REG(r) (cpu.addr_reg[(((r) < OP_MEM ? (r) : (r - 1))) >> 1])

cpu_t cpu;

static byte_t get_code(void)
{
    return mem_read(cpu.pc++);
}

static void push8(byte_t data)
{
    mem_write(--cpu.sp, data);
}

static byte_t pop8(void)
{
    return mem_read(cpu.sp++);
}

static void push16(addr_t data)
{
    push8(GETH(data));
    push8(GETL(data));
}

static addr_t pop16(void)
{
    addr_t data = 0;
    data = SETL(data, pop8());
    data = SETH(data, pop8());
    return data;
}

// returns what A register should be
static unsigned setflags(unsigned result)
{
    cpu.z = (result == 0);
    cpu.s = (result > 0x7F);
    {
        unsigned p = result;
        p = (p >> 4) ^ p;
        p = (p >> 2) ^ p;
        p = (p >> 1) ^ p;
        cpu.p = p ^ 1;
    }
    cpu.cy = (result > 0xFF);
    return result;
}

static unsigned setflags_sum(unsigned a, unsigned b)
{
    unsigned result = a + b;
    setflags(result);
    cpu.ac = (((a & 0xF) + (b & 0xF)) >> 4);
    return result;
}

static void op_write(byte_t op, byte_t data)
{
    if (op == OP_MEM)
        mem_write(cpu.hl, data);
    else
        BYTE_REG(op) = data;
}

static byte_t op_read(byte_t op)
{
    if (op == OP_MEM)
        return mem_read(cpu.hl);
    else
        return BYTE_REG(op);
}

void cpu_step(void)
{
    if (cpu.halt)
        return;

    const byte_t code = get_code();
    unsigned r = 0;

    switch (code)
    {
        // Add Immediate To Accumulator
        case INSTR_ADI:
            cpu.a = setflags_sum(cpu.a, get_code());
            return;
        // Add Immediate To Accumulator With Carry
        case INSTR_ACI:
            cpu.a = setflags_sum(cpu.a, get_code() + cpu.cy);
            return;
        // Subtract Immediate From Accumulator
        case INSTR_SUI:
            cpu.a = setflags_sum(cpu.a, -get_code());
            return;
        // Subtract Immediate From Accumulator With Borrow
        case INSTR_SBI:
            cpu.a = setflags_sum(cpu.a, -get_code() - cpu.cy);
            return;
        // And Immediate With Accumulator
        case INSTR_ANI:
            cpu.a = setflags(cpu.a & get_code());
            return;
        // Exclusive-Or Immediate With Accumulator
        case INSTR_XRI:
            cpu.a = setflags(cpu.a ^ get_code());
            return;
        // Or Immediate With Accumulator
        case INSTR_ORI:
            cpu.a = setflags(cpu.a | get_code());
            return;
        // Compare Immediate With Accumulator
        case INSTR_CPI:
            setflags_sum(cpu.a, -get_code());
            return;
        // Rotate Accumulator Left
        case INSTR_RLC:
            cpu.cy = (cpu.a > 0x7F);
            // Fall-thru
        // Rotate Accumulator Left Through Carry
        case INSTR_RAL:
            r = ((cpu.a << 1) | cpu.cy);
            cpu.cy = (cpu.a > 0x7F);
            cpu.a = r;
            return;
        // Rotate Accumulator Right
        case INSTR_RRC:
            cpu.cy = (cpu.a & 1);
            // Fall-thru
        // Rotate Accumulator Right Through Carry
        case INSTR_RAR:
            r = ((cpu.a >> 1) | (cpu.cy << 7));
            cpu.cy = (cpu.a & 1);
            cpu.a = r;
            return;
        // JUMP
        case INSTR_JMP:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            cpu.pc = r;
            return;
        // Jump If Carry
        case INSTR_JC:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.cy)
                cpu.pc = r;
            return;
        // Jump If No Carry
        case INSTR_JNC:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.cy)
                cpu.pc = r;
            return;
        // Jump If Zero
        case INSTR_JZ:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.z)
                cpu.pc = r;
            return;
        // Jump If Not Zero
        case INSTR_JNZ:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.z)
                cpu.pc = r;
            return;
        // Jump If Minus
        case INSTR_JM:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.s)
                cpu.pc = r;
            return;
         // Jump If Positive
        case INSTR_JP:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.s)
                cpu.pc = r;
            return;
       // Jump If Parity Even
        case INSTR_JPE:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.p)
                cpu.pc = r;
            return;
        // Jump If Parity Odd
        case INSTR_JPO:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.p)
                cpu.pc = r;
            return;
        // Call
        case INSTR_CALL:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            push16(cpu.pc);
            cpu.pc = r;
            return;
        // Call If Carry
        case INSTR_CC:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.cy)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If No Carry
        case INSTR_CNC:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.cy)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If Zero
        case INSTR_CZ:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.z)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If Not Zero
        case INSTR_CNZ:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.z)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If Minus
        case INSTR_CM:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.s)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
         // Call If Plus
        case INSTR_CP:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.s)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If Parity Even
        case INSTR_CPE:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (cpu.p)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Call If Parity Odd
        case INSTR_CPO:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            if (!cpu.p)
            {
                push16(cpu.pc);
                cpu.pc = r;
            }
            return;
        // Return
        case INSTR_RET:
            cpu.pc = pop16();
            return;
        // Return If Carry
        case INSTR_RC:
            if (cpu.cy)
                cpu.pc = pop16();
            return;
        // Return If No Carry
        case INSTR_RNC:
            if (!cpu.cy)
                cpu.pc = pop16();
            return;
        // Return If Zero
        case INSTR_RZ:
            if (cpu.z)
                cpu.pc = pop16();
            return;
        // Return If Not Zero
        case INSTR_RNZ:
            if (!cpu.z)
                cpu.pc = pop16();
            return;
        // Return If Minus
        case INSTR_RM:
            if (cpu.s)
                cpu.pc = pop16();
            return;
         // Return If Plus
        case INSTR_RP:
            if (!cpu.s)
                cpu.pc = pop16();
            return;
        // Return If Parity Even
        case INSTR_RPE:
            if (cpu.p)
                cpu.pc = pop16();
            return;
        // Return If Parity Odd
        case INSTR_RPO:
            if (!cpu.p)
                cpu.pc = pop16();
            return;
        // Input
        case INSTR_IN:
            cpu.a = io_read(get_code());
            return;
        // Output
        case INSTR_OUT:
            io_write(get_code(), cpu.a);
            return;
        // Load Immediate Into Register Pair (b,c)
        case INSTR_LXIB:
            cpu.bc = SETL(cpu.bc, get_code());
            cpu.bc = SETH(cpu.bc, get_code());
            return;
        // Load Immediate Into Register Pair (d,e)
        case INSTR_LXID:
            cpu.de = SETL(cpu.de, get_code());
            cpu.de = SETH(cpu.de, get_code());
            return;
        // Load Immediate Into Register Pair (hl)
        case INSTR_LXIH:
            cpu.hl = SETL(cpu.hl, get_code());
            cpu.hl = SETH(cpu.hl, get_code());
            return;
        // Load Immediate Into Register Pair (sp)
        case INSTR_LXISP:
            cpu.sp = SETL(cpu.sp, get_code());
            cpu.sp = SETH(cpu.sp, get_code());
            return;
         // Push Data Onto Stack (b,c)
        case INSTR_PUSHB:
            push16(cpu.bc);
            return;
        // Push Data Onto Stack (d,e)
        case INSTR_PUSHD:
            push16(cpu.de);
            return;
         // Push Data Onto Stack (h,l)
        case INSTR_PUSHH:
            push16(cpu.hl);
            return;
         // Push Data Onto Stack (a,f)
        case INSTR_PUSHPSW:
            push16(cpu.af);
            return;
        // Pop Data Off Stack (b,c)
        case INSTR_POPB:
            cpu.bc = pop16();
            return;
        // Pop Data Off Stack (d,e)
        case INSTR_POPD:
            cpu.de = pop16();
            return;
        // Pop Data Off Stack (h,l)
        case INSTR_POPH:
            cpu.hl = pop16();
            return;
        // Pop Data Off Stack (a,f)
        case INSTR_POPPSW:
            cpu.af = pop16();
            return;
        // Store Accumulator Direct
        case INSTR_STA:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            mem_write(r, cpu.a);
            return;
        // Load Accumulator Direct
        case INSTR_LDA:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            cpu.a = mem_read(r);
            return;
        // Exchange Registers
        case INSTR_XCHG:
            r = cpu.hl;
            cpu.hl = cpu.de;
            cpu.de = r;
            return;
        // Exchange Stack
        case INSTR_XTHL:
            r = cpu.l;
            cpu.l = mem_read(cpu.sp + 0);
            mem_write(cpu.sp + 0, r);
            r = cpu.h;
            cpu.h = mem_read(cpu.sp + 1);
            mem_write(cpu.sp + 1, r);
            return;
        // Load SP From H And L
        case INSTR_SPHL:
            cpu.sp = cpu.hl;
            return;
        // Load Program Counter
        case INSTR_PCHL:
            cpu.pc = cpu.hl;
            return;
        // Double Add (b,c)
        case INSTR_DADB:
            r = cpu.hl + cpu.bc;
            cpu.cy = (r > 0xFFFF);
            cpu.hl = r;
            return;
        // Double Add (d,e)
        case INSTR_DADD:
            r = cpu.hl + cpu.de;
            cpu.cy = (r > 0xFFFF);
            cpu.hl = r;
            return;
         // Double Add (h,l)
        case INSTR_DADH:
            r = cpu.hl + cpu.hl;
            cpu.cy = (r > 0xFFFF);
            cpu.hl = r;
            return;
         // Double Add (sp)
        case INSTR_DADSP:
            r = cpu.hl + cpu.sp;
            cpu.cy = (r > 0xFFFF);
            cpu.hl = r;
            return;
         // Store Accumulator (to (B,C))
        case INSTR_STAXB:
            mem_write(cpu.bc, cpu.a);
            return;
        // Store Accumulator (to (D,E))
        case INSTR_STAXD:
            mem_write(cpu.de, cpu.a);
            return;
        // Load Accumulator (from (B,C))
        case INSTR_LDAXB:
            cpu.a = mem_read(cpu.bc);
            return;
        // Load Accumulator (from (D,E))
        case INSTR_LDAXD:
            cpu.a = mem_read(cpu.de);
            return;
        // Increment Register Pair (b,c)
        case INSTR_INXB:
            cpu.bc++;
            return;
        // Increment Register Pair (d,e)
        case INSTR_INXD:
            cpu.de++;
            return;
         // Increment Register Pair (h,l)
        case INSTR_INXH:
            cpu.hl++;
            return;
         // Increment Register Pair (sp)
        case INSTR_INXSP:
            cpu.sp++;
            return;
        // Decrement Register Pair (b,c)
        case INSTR_DCXB:
            cpu.bc--;
            return;
        // Decrement Register Pair (d,e)
        case INSTR_DCXD:
            cpu.de--;
            return;
         // Decrement Register Pair (h,l)
        case INSTR_DCXH:
            cpu.hl--;
            return;
         // Decrement Register Pair (sp)
        case INSTR_DCXSP:
            cpu.sp--;
            return;
         // Complement Accumulator
        case INSTR_CMA:
            cpu.a = ~cpu.a;
            return;
        // Set Carry
        case INSTR_STC:
            cpu.cy = 1;
            return;
        // Complement Carry
        case INSTR_CMC:
            cpu.cy = ~cpu.cy;
            return;
        // Decimal Adjust Accumulator
        case INSTR_DAA:
            if (cpu.ac || (cpu.a & 0xF) > 9)
                r += 6;
            if (cpu.cy || (cpu.a >> 4) > 9)
                r += (6 << 4);
            cpu.a = setflags_sum(cpu.a, r);
            return;
        // Store H and L Direct
        case INSTR_SHLD:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            mem_write(r + 0, cpu.l);
            mem_write(r + 1, cpu.h);
            return;
        // Load H and L Direct
        case INSTR_LHLD:
            r = SETL(r, get_code());
            r = SETH(r, get_code());
            cpu.l = mem_read(r + 0);
            cpu.h = mem_read(r + 1);
            return;
        // Enable Interrupts
        case INSTR_EI:
            cpu.inte = 1;
            return;
        // Disable Interrupts
        case INSTR_DI:
            cpu.inte = 0;
            return;
        // HALT INSTRUCTION
        case INSTR_HLT:
            cpu.halt = 1;
            return;
        // NOP INSTRUCTIONS
        case INSTR_NOP:
            return;
    }

    switch (code & LOW_OP_MASK)
    {
        // ADD Register or Memory To Accumulator
        case INSTRBITS_ADD:
            cpu.a = setflags_sum(cpu.a, op_read(OP_2(code)));
            return;
        // ADD Register or Memory To Accumulator With Carry
        case INSTRBITS_ADC:
            cpu.a = setflags_sum(cpu.a, op_read(OP_2(code) + cpu.cy));
            return;
        // Subtract Register or Memory From Accumulator
        case INSTRBITS_SUB:
            cpu.a = setflags_sum(cpu.a, -op_read(OP_2(code)));
            return;
        // Subtract Register or Memory From Accumulator With Borrow
        case INSTRBITS_SBB:
            cpu.a = setflags_sum(cpu.a, -op_read(OP_2(code)) - cpu.cy);
            return;
        // Logical and Register or Memory With Accumulator
        case INSTRBITS_ANA:
            cpu.a = setflags(cpu.a & op_read(OP_2(code)));
            return;
        // Logical Exclusive-Or Register or Memory With Accumulator
        case INSTRBITS_XRA:
            // Affects AC for some reason
            cpu.a = setflags_sum(cpu.a ^ op_read(OP_2(code)), 0);
            return;
         // Logical or Register or Memory With Accumulator
        case INSTRBITS_ORA:
            cpu.a = setflags(cpu.a | op_read(OP_2(code)));
            return;
       // Compare Register or Memory With Accumulator
        case INSTRBITS_CMP:
            setflags_sum(cpu.a, -op_read(OP_2(code)));
            return;
    }

    switch (code & HIGH_OP_MASK)
    {
        // Move Immediate Data
        case INSTRBITS_MVI:
            op_write(OP_1(code), get_code());
            return;
        // Increment Register or Memory
        case INSTRBITS_INR:
            op_write(OP_1(code), setflags_sum(op_read(OP_1(code)), 1));
            return;
        // Decrement Register or Memory
        case INSTRBITS_DCR:
            op_write(OP_1(code), setflags_sum(op_read(OP_1(code)), -1));
            return;
        // Restart
        case INSTRBITS_RST:
            push16(cpu.pc);
            cpu.pc = 8 * OP_1(code);
            return;
    }

    switch (code & BOTH_OP_MASK)
    {
        // MOV Instruction
        case INSTRBITS_MOV:
            op_write(OP_1(code), op_read(OP_2(code)));
            return;
    }
}
