#pragma once

#include <map>  

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

    ,
    OP_BRN, // branch if negative
    OP_BRZ, // branch if zero
    OP_BRP, // branch if positive
    OP_BRNZ, // branch if not zero
    OP_BRNP, // branch if not positive
    OP_BRZP, // branch if not zero

    // pseudo opcodes (to be implemented)
    OP_BREQ, // branch if equal 
    OP_BRNE, // branch if not equal
    OP_BRGT, // branch if greater than
    OP_BRGE, // branch if greater than or equal
    OP_BRLT, // branch if less than
    OP_BRLE, // branch if less than or equals
    OP_BREQZ, // branch if equal to zero
    OP_BRNEZ, // branch if not equal to zero
    OP_RET, // return
    OP_OR, // bitwise OR
    OP_JSRR, // Jump Subroutine Register
};

