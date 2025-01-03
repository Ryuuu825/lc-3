#include <iostream>
#include <fstream>

// define lc-3 machine data structure
#include "lc_type.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "op_func.hpp"

Memory memory;
lc_word_t reg[R_COUNT] = {0};
bool running = true;

extern op_func op_table[16];

int main(int argc, char** argv)
{
    std::fstream file("../assembler/test.bin", std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        return 1;
    }

    lc_word_t instr;
    file.read(reinterpret_cast<char*>(&instr), sizeof(lc_word_t)); // starting address stored in the first word
    lc_uint_t i = instr;

    reg[R_PC] = i;
    reg[R_COND] = FL_ZRO;

    while (file.read(reinterpret_cast<char*>(&instr), sizeof(lc_word_t)))
    {
        memory.write(i++, instr);
    }

    while(running && reg[R_PC] < 0x8000)
    {
        // Fetch
        lc_word_t instr = memory.read(reg[R_PC]);
        // std::cout << "instr: " << std::hex << instr << std::endl;
        // Decode
        Opcode op = (Opcode)(instr >> 12);
        // Execute
        op_table[op](instr);
        reg[R_PC]++;
    }
}