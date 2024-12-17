#include "lc_type.hpp"
#include "op_func.hpp"

extern bool running;
extern Memory memory;
extern lc_word_t reg[R_COUNT];
extern void (*trap_table[16])();

lc_int_t sign_extend(lc_int_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1)
    {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

lc_uint_t bit_at(lc_uint_t x, int bit)
{
    return (x >> bit) & 1;
}

void update_flag(lc_uint_t r)
{
    if (reg[r] == 0)
    {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15)
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}

#include <iostream>

void op_add(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9 ) & 0b111;
    lc_uint_t sr1 = (instr >> 6) & 0b111;
    lc_uint_t imm_flag = bit_at(instr, 5);
    if (imm_flag) 
    {
        lc_int_t imm5 = sign_extend(instr & 0b11111, 5);
        reg[dr] = reg[sr1] + imm5;
    }
    else 
    {
        lc_uint_t sr2 = instr & 0b111;
        reg[dr] = reg[sr1] + reg[sr2];
    }
    update_flag(dr);
}


void op_and(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_uint_t sr1 = (instr >> 6) & 0b111;
    lc_uint_t imm_flag = bit_at(instr, 5);
    if (imm_flag)
    {
        lc_int_t imm5 = sign_extend(instr & 0b11111, 5);
        reg[dr] = reg[sr1] & imm5;
    }
    else
    {
        lc_uint_t sr2 = instr & 0b111;
        reg[dr] = reg[sr1] & reg[sr2];
    }
    update_flag(dr);
}

void op_not(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_uint_t sr = (instr >> 6) & 0b111;
    reg[dr] = ~reg[sr];
    update_flag(dr);
}

void op_br(lc_word_t instr)
{
    lc_uint_t n = bit_at(instr, 11); // Negative
    lc_uint_t z = bit_at(instr, 10); // Zero
    lc_uint_t p = bit_at(instr, 9);  // Positive
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    if ((n && (reg[R_COND] & FL_NEG)) || (z && (reg[R_COND] & FL_ZRO)) || (p && (reg[R_COND] & FL_POS)) || (!n && !z && !p))
    {
        reg[R_PC] += pc_offset;
    }
}

void op_jmp(lc_word_t instr)
{
    lc_uint_t base_r = (instr >> 6) & 0b111;
    reg[R_PC] = reg[base_r];
}

void op_jsr(lc_word_t instr)
{
    lc_uint_t long_flag = bit_at(instr, 11);
    reg[R_R7] = reg[R_PC];
    if (long_flag)
    {
        lc_int_t pc_offset = sign_extend(instr & 0x7FF, 11);
        reg[R_PC] += pc_offset;
    }
    else
    {
        lc_uint_t base_r = (instr >> 6) & 0b111;
        reg[R_PC] = reg[base_r];
    }
}

void op_trap(lc_word_t instr)
{

    TrapCode trap_vector = (TrapCode) (instr & 0xFF);
    trap_table[trap_vector]();
}

void op_ld(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[dr] = memory.read(reg[R_PC] + pc_offset);
    update_flag(dr);
}

void op_ldi(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[dr] = memory.read(memory.read(reg[R_PC] + pc_offset));
    update_flag(dr);
}

void op_ldr(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_uint_t base_r = (instr >> 6) & 0b111;
    lc_int_t offset = sign_extend(instr & 0x3F, 6);
    reg[dr] = memory.read(reg[base_r] + offset);
    update_flag(dr);
}

void op_lea(lc_word_t instr)
{
    lc_uint_t dr = (instr >> 9) & 0b111;
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    reg[dr] = reg[R_PC] + pc_offset;
    update_flag(dr);
}

void op_st(lc_word_t instr)
{
    lc_uint_t sr = (instr >> 9) & 0b111;
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    memory.write(reg[R_PC] + pc_offset, reg[sr]);
}

void op_sti(lc_word_t instr)
{
    lc_uint_t sr = (instr >> 9) & 0b111;
    lc_int_t pc_offset = sign_extend(instr & 0x1FF, 9);
    memory.write(memory.read(reg[R_PC] + pc_offset), reg[sr]);
}

void op_str(lc_word_t instr)
{
    lc_uint_t sr = (instr >> 9) & 0b111;
    lc_uint_t base_r = (instr >> 6) & 0b111;
    lc_int_t offset = sign_extend(instr & 0x3F, 6);
    memory.write(reg[base_r] + offset, reg[sr]);
}

void op_res(lc_word_t instr)
{
    reg[R_PC] = reg[R_R7];
}

void op_rti(lc_word_t instr)
{
    printf("RTI is not implemented\n");
}


// op table: maps opcode to a function
op_func op_table[16] = {
    op_br, op_add, op_ld, op_st,
    op_jsr, op_and, op_ldr, op_str,
    op_rti, op_not, op_ldi, op_sti,
    op_jmp, op_res, op_lea, op_trap
};
