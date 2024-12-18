#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector> 
#include <map>

#include "../virutal_machine/include/lc_type.hpp"

#include "opcode.hpp"
#include "op_func.hpp"
#include "stringhelper.hpp"

extern std::map<std::string, Opcode> opcodes;
extern std::map<Opcode, lc_word_t (*)(std::vector<std::string>)> op_table;

int main(int argc, char** argv)
{
    std::ifstream file("test.asm");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        lines.push_back(trim(line));
    }

    std::vector<lc_word_t> memory;
    memory.push_back(0x0000); // placeholder for entry_address
    memory.push_back(0x0000); // placeholder for program_size

    // first pass
    std::map<std::string, lc_word_t> symbol_table;
    lc_word_t address = 0; // logical address
    for (const auto& line : lines)
    {
        if (line[0] == '.') 
        {
            // ORIG
        };
        if (line[0] == ' ')
        {
            address++;
            continue;
        }
        std::vector<std::string> tokens = split(remove_after(line, ';'), " ");
        if (tokens.size() == 1)
        {
            symbol_table[tokens[0]] = address;
        }
        else
        {
            address++;
        }
    }

    // second pass
    for (const auto& line : lines)
    {
        if (line[0] == '.') continue;
        std::vector<std::string> tokens = split(remove_after(line, ';'), " ");

        // parse instruction
        Opcode opcode = opcodes[tokens[0]];
        lc_word_t instr = 0;
        instr |= opcode << 12;

        // map to function
        instr |= (op_table[opcode](tokens) & 0x0FFF);

        memory.push_back(instr);
    }

    // write to file
    std::ofstream out("test.bin", std::ios::binary);
    for (const auto& instr : memory)
    {
        out.write(reinterpret_cast<const char*>(&instr), sizeof(lc_word_t));
    }

}