#include <map>
#include <iostream>

#include "trap.hpp"

extern bool running;
extern Memory memory;
extern lc_word_t reg[R_COUNT];

void trap_getc()
{
    reg[R_R0] = (lc_uint_t)getchar();
    reg[R_R0] &= 0x0F;
}

void trap_puts()
{
    lc_uint_t* c = memory.main_memory.data() + reg[R_R0];
    while (*c)
    {
        // two char in one word
        std::cout << (char)(*c & 0xFF);
        std::cout << (char)(*c >> 8);
        c++;
    }
}

void trap_out()
{
    std::cout << (char)reg[R_R0] << std::flush;
}

void trap_out_int()
{
    std::cout << (int)reg[R_R0] << std::flush;
}

void trap_halt()
{    
    std::cout << "Program halted" << std::endl;
    running = false;
}

std::map<int, trap_func> trap_table = {
    {0x20, trap_getc},
    {0x21, trap_out},
    {0x22, trap_puts},
    {0x24, trap_out_int},
    {0x25, trap_halt},
};
