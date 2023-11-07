#include <stdio.h>
#include "definitions.h"
#include "mem.h"

static addr_t disas_addr;

static byte_t get_byte(void)
{
    return mem[disas_addr++];
}

static addr_t get_word(void)
{
    addr_t r = 0;
    r = SETL(r, mem[disas_addr++]);
    r = SETH(r, mem[disas_addr++]);
    return r;
}

static const char *op_str(byte_t op)
{
    switch (op)
    {
        case OP_A   : return "a";
        case OP_B   : return "b";
        case OP_C   : return "c";
        case OP_D   : return "d";
        case OP_E   : return "e";
        case OP_H   : return "h";
        case OP_L   : return "l";
        case OP_MEM : return "(hl)";
        default     : return "?";
    }
}

addr_t disas(char *s, addr_t addr)
{
    disas_addr = addr;

    const byte_t code = get_byte();

    switch (code)
    {
        // Add Immediate To Accumulator
        case INSTR_ADI:
            sprintf(s, "adi     %i", get_byte());
            return disas_addr;
        // Add Immediate To Accumulator With Carry
        case INSTR_ACI:
            sprintf(s, "aci     %i", get_byte());
            return disas_addr;
        // Subtract Immediate From Accumulator
        case INSTR_SUI:
            sprintf(s, "sui     %i", get_byte());
            return disas_addr;
        // Subtract Immediate From Accumulator With Borrow
        case INSTR_SBI:
            sprintf(s, "sbi     %i", get_byte());
            return disas_addr;
        // And Immediate With Accumulator
        case INSTR_ANI:
            sprintf(s, "ani     %i", get_byte());
            return disas_addr;
        // Exclusive-Or Immediate With Accumulator
        case INSTR_XRI:
            sprintf(s, "xri     %i", get_byte());
            return disas_addr;
        // Or Immediate With Accumulator
        case INSTR_ORI:
            sprintf(s, "ori     %i", get_byte());
            return disas_addr;
        // Compare Immediate With Accumulator
        case INSTR_CPI:
            sprintf(s, "cpi     %i", get_byte());
            return disas_addr;
        // Rotate Accumulator Left
        case INSTR_RAL:
            sprintf(s, "ral");
            return disas_addr;
        // Rotate Accumulator Left Through Carry
        case INSTR_RLC:
            sprintf(s, "rlc");
           return disas_addr;
        // Rotate Accumulator Right
        case INSTR_RAR:
            sprintf(s, "rar");
            return disas_addr;
        // Rotate Accumulator Right Through Carry
        case INSTR_RRC:
            sprintf(s, "rrc");
            return disas_addr;
        // JUMP
        case INSTR_JMP:
            sprintf(s, "jmp     0x%04x", get_word());
            return disas_addr;
        // Jump If Carry
        case INSTR_JC:
            sprintf(s, "jc      0x%04x", get_word());
            return disas_addr;
        // Jump If No Carry
        case INSTR_JNC:
            sprintf(s, "jnc     0x%04x", get_word());
            return disas_addr;
        // Jump If Zero
        case INSTR_JZ:
            sprintf(s, "jz      0x%04x", get_word());
            return disas_addr;
        // Jump If Not Zero
        case INSTR_JNZ:
            sprintf(s, "jnz     0x%04x", get_word());
            return disas_addr;
        // Jump If Minus
        case INSTR_JM:
            sprintf(s, "jm      0x%04x", get_word());
            return disas_addr;
         // Jump If Positive
        case INSTR_JP:
            sprintf(s, "jp      0x%04x", get_word());
            return disas_addr;
       // Jump If Parity Even
        case INSTR_JPE:
            sprintf(s, "jpe     0x%04x", get_word());
            return disas_addr;
        // Jump If Parity Odd
        case INSTR_JPO:
            sprintf(s, "jpo     0x%04x", get_word());
            return disas_addr;
        // Call
        case INSTR_CALL:
            sprintf(s, "call    0x%04x", get_word());
            return disas_addr;
        // Call If Carry
        case INSTR_CC:
            sprintf(s, "cc      0x%04x", get_word());
            return disas_addr;
        // Call If No Carry
        case INSTR_CNC:
            sprintf(s, "cnc     0x%04x", get_word());
            return disas_addr;
        // Call If Zero
        case INSTR_CZ:
            sprintf(s, "cz      0x%04x", get_word());
            return disas_addr;
        // Call If Not Zero
        case INSTR_CNZ:
            sprintf(s, "cnz     0x%04x", get_word());
            return disas_addr;
        // Call If Minus
        case INSTR_CM:
            sprintf(s, "cm      0x%04x", get_word());
            return disas_addr;
         // Call If Plus
        case INSTR_CP:
            sprintf(s, "cp      0x%04x", get_word());
            return disas_addr;
        // Call If Parity Even
        case INSTR_CPE:
            sprintf(s, "cpe     0x%04x", get_word());
            return disas_addr;
        // Call If Parity Odd
        case INSTR_CPO:
            sprintf(s, "cpo     0x%04x", get_word());
            return disas_addr;
        // Return
        case INSTR_RET:
            sprintf(s, "ret");
            return disas_addr;
        // Return If Carry
        case INSTR_RC:
            sprintf(s, "rc");
            return disas_addr;
        // Return If No Carry
        case INSTR_RNC:
            sprintf(s, "rnc");
            return disas_addr;
        // Return If Zero
        case INSTR_RZ:
            sprintf(s, "rz");
            return disas_addr;
        // Return If Not Zero
        case INSTR_RNZ:
            sprintf(s, "rnz");
            return disas_addr;
        // Return If Minus
        case INSTR_RM:
            sprintf(s, "rm");
            return disas_addr;
         // Return If Plus
        case INSTR_RP:
            sprintf(s, "rp");
            return disas_addr;
        // Return If Parity Even
        case INSTR_RPE:
            sprintf(s, "rpe");
            return disas_addr;
        // Return If Parity Odd
        case INSTR_RPO:
            sprintf(s, "rpo");
            return disas_addr;
        // Input
        case INSTR_IN:
            sprintf(s, "in");
            return disas_addr;
        // Output
        case INSTR_OUT:
            sprintf(s, "out");
            return disas_addr;
        // Load Immediate Into Register Pair (b,c)
        case INSTR_LXIB:
            sprintf(s, "lxi     bc, %i", get_word());
            return disas_addr;
        // Load Immediate Into Register Pair (d,e)
        case INSTR_LXID:
            sprintf(s, "lxi     de, %i", get_word());
            return disas_addr;
        // Load Immediate Into Register Pair (hl)
        case INSTR_LXIH:
            sprintf(s, "lxi     hl, %i", get_word());
            return disas_addr;
        // Load Immediate Into Register Pair (sp)
        case INSTR_LXISP:
            sprintf(s, "lxi     sp, %i", get_word());
            return disas_addr;
         // Push Data Onto Stack (b,c)
        case INSTR_PUSHB:
            sprintf(s, "push    bc");
            return disas_addr;
        // Push Data Onto Stack (d,e)
        case INSTR_PUSHD:
            sprintf(s, "push    de");
            return disas_addr;
         // Push Data Onto Stack (h,l)
        case INSTR_PUSHH:
            sprintf(s, "push    hl");
            return disas_addr;
         // Push Data Onto Stack (a,f)
        case INSTR_PUSHPSW:
            sprintf(s, "push    psw");
            return disas_addr;
        // Pop Data Off Stack (b,c)
        case INSTR_POPB:
            sprintf(s, "pop     bc");
            return disas_addr;
        // Pop Data Off Stack (d,e)
        case INSTR_POPD:
            sprintf(s, "pop     de");
            return disas_addr;
        // Pop Data Off Stack (h,l)
        case INSTR_POPH:
            sprintf(s, "pop     hl");
            return disas_addr;
        // Pop Data Off Stack (a,f)
        case INSTR_POPPSW:
            sprintf(s, "pop     psw");
            return disas_addr;
        // Store Accumulator Direct
        case INSTR_STA:
            sprintf(s, "sta     0x%04x", get_word());
            return disas_addr;
        // Load Accumulator Direct
        case INSTR_LDA:
            sprintf(s, "lda     0x%04x", get_word());
            return disas_addr;
        // Exchange Registers
        case INSTR_XCHG:
            sprintf(s, "xchg");
            return disas_addr;
        // Exchange Stack
        case INSTR_XTHL:
            sprintf(s, "xthl");
            return disas_addr;
        // Load SP From H And L
        case INSTR_SPHL:
            sprintf(s, "sphl");
            return disas_addr;
        // Load Program Counter
        case INSTR_PCHL:
            sprintf(s, "pchl");
            return disas_addr;
        // Double Add (b,c)
        case INSTR_DADB:
            sprintf(s, "dad     bc");
            return disas_addr;
        // Double Add (d,e)
        case INSTR_DADD:
            sprintf(s, "dad     de");
            return disas_addr;
         // Double Add (h,l)
        case INSTR_DADH:
            sprintf(s, "dad     hl");
            return disas_addr;
         // Double Add (sp)
        case INSTR_DADSP:
            sprintf(s, "dad     sp");
            return disas_addr;
         // Store Accumulator (to (B,C))
        case INSTR_STAXB:
            sprintf(s, "stax    bc");
            return disas_addr;
        // Store Accumulator (to (D,E))
        case INSTR_STAXD:
            sprintf(s, "stax    de");
            return disas_addr;
        // Load Accumulator (from (B,C))
        case INSTR_LDAXB:
            sprintf(s, "ldax    bc");
            return disas_addr;
        // Load Accumulator (from (D,E))
        case INSTR_LDAXD:
            sprintf(s, "ldax    de");
            return disas_addr;
        // Increment Register Pair (b,c)
        case INSTR_INXB:
            sprintf(s, "inx     bc");
            return disas_addr;
        // Increment Register Pair (d,e)
        case INSTR_INXD:
            sprintf(s, "inx     de");
            return disas_addr;
         // Increment Register Pair (h,l)
        case INSTR_INXH:
            sprintf(s, "inx     hl");
            return disas_addr;
         // Increment Register Pair (sp)
        case INSTR_INXSP:
            sprintf(s, "inx     sp");
            return disas_addr;
        // Decrement Register Pair (b,c)
        case INSTR_DCXB:
            sprintf(s, "dcx     bc");
            return disas_addr;
        // Decrement Register Pair (d,e)
        case INSTR_DCXD:
            sprintf(s, "dcx     de");
            return disas_addr;
         // Decrement Register Pair (h,l)
        case INSTR_DCXH:
            sprintf(s, "dcx     hl");
            return disas_addr;
         // Decrement Register Pair (sp)
        case INSTR_DCXSP:
            sprintf(s, "dcx     sp");
            return disas_addr;
         // Complement Accumulator
        case INSTR_CMA:
            sprintf(s, "cma");
            return disas_addr;
        // Set Carry
        case INSTR_STC:
            sprintf(s, "stc");
            return disas_addr;
        // Complement Carry
        case INSTR_CMC:
            sprintf(s, "cmc");
            return disas_addr;
        // Decimal Adjust Accumulator
        case INSTR_DAA:
            sprintf(s, "daa");
            return disas_addr;
        // Store H and L Direct
        case INSTR_SHLD:
            sprintf(s, "shld    0x%04x", get_word());
            return disas_addr;
        // Load H and L Direct
        case INSTR_LHLD:
            sprintf(s, "lhld    0x%04x", get_word());
            return disas_addr;
        // Enable Interrupts
        case INSTR_EI:
            sprintf(s, "ei");
            return disas_addr;
        // Disable Interrupts
        case INSTR_DI:
            sprintf(s, "di");
            return disas_addr;
        // HALT INSTRUCTION
        case INSTR_HLT:
            sprintf(s, "hlt");
            return disas_addr;
        // NOP INSTRUCTIONS
        case INSTR_NOP:
            sprintf(s, "nop");
            return disas_addr;
    }

    switch (code & LOW_OP_MASK)
    {
        // ADD Register or Memory To Accumulator
        case INSTRBITS_ADD:
            sprintf(s, "add     %s", op_str(OP_2(code)));
            return disas_addr;
        // ADD Register or Memory To Accumulator With Carry
        case INSTRBITS_ADC:
            sprintf(s, "adc     %s", op_str(OP_2(code)));
            return disas_addr;
        // Subtract Register or Memory From Accumulator
        case INSTRBITS_SUB:
            sprintf(s, "sub     %s", op_str(OP_2(code)));
            return disas_addr;
        // Subtract Register or Memory From Accumulator With Borrow
        case INSTRBITS_SBB:
            sprintf(s, "sbb     %s", op_str(OP_2(code)));
            return disas_addr;
        // Logical and Register or Memory With Accumulator
        case INSTRBITS_ANA:
            sprintf(s, "ana     %s", op_str(OP_2(code)));
            return disas_addr;
        // Logical Exclusive-Or Register or Memory With Accumulator
        case INSTRBITS_XRA:
            sprintf(s, "xra     %s", op_str(OP_2(code)));
            return disas_addr;
         // Logical or Register or Memory With Accumulator
        case INSTRBITS_ORA:
            sprintf(s, "ora     %s", op_str(OP_2(code)));
            return disas_addr;
       // Compare Register or Memory With Accumulator
        case INSTRBITS_CMP:
            sprintf(s, "cmp     %s", op_str(OP_2(code)));
            return disas_addr;
    }

    switch (code & HIGH_OP_MASK)
    {
        // Move Immediate Data
        case INSTRBITS_MVI:
            sprintf(s, "mvi     %s, %i", op_str(OP_1(code)), get_byte());
            return disas_addr;
        // Increment Register or Memory
        case INSTRBITS_INR:
            sprintf(s, "inr     %s", op_str(OP_1(code)));
            return disas_addr;
        // Decrement Register or Memory
        case INSTRBITS_DCR:
            sprintf(s, "dcr     %s", op_str(OP_1(code)));
            return disas_addr;
        // Restart
        case INSTRBITS_RST:
            sprintf(s, "rst     %i", OP_1(code));
            return disas_addr;
    }

    switch (code & BOTH_OP_MASK)
    {
        // MOV Instruction
        case INSTRBITS_MOV:
            sprintf(s, "mov     %s, %s", op_str(OP_1(code)), op_str(OP_2(code)));
            return disas_addr;
    }

    sprintf(s, "ud");
    return disas_addr;
}
