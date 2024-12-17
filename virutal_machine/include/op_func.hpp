#pragma once

#include "lc_type.hpp"
#include "trap.hpp"

enum Opcode
{
    OP_BR = 0b0, // Branch
    OP_ADD, // Add
    OP_LD, // Load
    OP_ST, // Store
    OP_JSR, // Jump Register
    OP_AND, // Bitwise AND
    OP_LDR, // Load Register
    OP_STR, // Store Register
    OP_RTI, // Return from Interrupt
    OP_NOT, // Bitwise NOT
    OP_LDI, // Load Indirect
    OP_STI, // Store Indirect
    OP_JMP, // Jump
    OP_RES, // Reserved (unused)
    OP_LEA, // Load Effective Address
    OP_TRAP // Execute Trap
};

// #section Functions
lc_int_t sign_extend(lc_int_t x, int bit_count);
lc_uint_t bit_at(lc_uint_t x, int bit);
void update_flag(lc_uint_t r);
void op_add(lc_word_t instr);
void op_and(lc_word_t instr);
void op_not(lc_word_t instr);
void op_br(lc_word_t instr);
void op_jmp(lc_word_t instr);
void op_jsr(lc_word_t instr);
void op_trap(lc_word_t instr);
void op_ld(lc_word_t instr);
void op_ldi(lc_word_t instr);
void op_ldr(lc_word_t instr);
void op_lea(lc_word_t instr);
void op_st(lc_word_t instr);
void op_sti(lc_word_t instr);
void op_str(lc_word_t instr);
void op_res(lc_word_t instr);
void op_rti(lc_word_t instr);

typedef void (*op_func)(lc_word_t);