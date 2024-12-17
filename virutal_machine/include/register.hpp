#pragma once

enum Register
{
    // 7 general purpose registers
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7, // Return Address

    R_PC, // Program Counter
    R_COND, // Condition Flag

    R_COUNT // Number of registers
};

enum ConditionFlag
{
    FL_POS = 1 << 0, // P
    FL_ZRO = 1 << 1, // Z
    FL_NEG = 1 << 2, // N
};
