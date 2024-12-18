#include <map>
#include <iostream>

#include "trap.hpp"

extern bool running;
extern lc_word_t reg[R_COUNT];

void trap_getc()
{
    reg[R_R0] = (lc_uint_t)getchar();
    reg[R_R0] &= 0x0F;
}

void trap_out()
{
    std::cout << (char)reg[R_R0] << std::flush;
}

void trap_out_int()
{
    std::cout << reg[R_R0] << std::flush;
}

void trap_halt()
{    
    std::cout << "Program halted" << std::endl;
    fflush(stdout);
    running = false;
}

std::map<int, trap_func> trap_table = {
    {0x20, trap_getc},
    {0x21, trap_out},
    {0x24, trap_out_int},
    {0x25, trap_halt},
};
