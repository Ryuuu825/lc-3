#include "trap.hpp"

#include <iostream>
extern bool running;
extern lc_word_t reg[R_COUNT];

void trap_getc()
{
    reg[R_R0] = (lc_uint_t)getchar();
}

void trap_out()
{
    std::cout << (char)reg[R_R0] << std::flush;
}

void trap_in()
{
    char c = getchar();
    putc(c, stdout);
    reg[R_R0] = c & 0x0F;
}

void trap_halt()
{    
    std::cout << "Program halted" << std::endl;
    fflush(stdout);
    running = false;
}

trap_func trap_table[16] = {
    trap_getc,
    trap_out, 
    trap_in, 
    trap_halt
};
