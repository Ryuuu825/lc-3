#pragma once

#include <cstdio>
#include "lc_type.hpp"
#include "register.hpp"
#include "memory.hpp"

enum TrapCode
{
    TRAP_GETC = 0x0, // Get character from keyboard, not echoed onto the terminal
    TRAP_OUT, // Output a character
    TRAP_IN, // Get character from keyboard, echoed onto the terminal
    TRAP_HALT, // Halt the program
};

void trap_getc();
void trap_out();
void trap_in();
void trap_halt();

typedef void (*trap_func)(void);
