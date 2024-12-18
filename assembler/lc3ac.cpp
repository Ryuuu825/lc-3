#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector> 
#include <map>

#include "../virutal_machine/include/lc_type.hpp"

#include "opcode.hpp"
#include "op_func.hpp"
#include "directives.hpp"
#include "stringhelper.hpp"

extern std::map<std::string, Opcode> opcodes;
extern std::map<Opcode, std::vector<lc_word_t> (*)(std::vector<std::string>)> op_table;
extern std::map<std::string, CompilerDirectives> directives;
extern std::map<CompilerDirectives, void (*)(std::string)> directive_table;

std::vector<lc_word_t> memory;

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

    memory.push_back(0x0000); // placeholder for entry_address
    memory.push_back(0x0000); // placeholder for program_size

    // first pass
    std::map<std::string, lc_word_t> symbol_table;
    lc_word_t address = 0; // logical address
    for (const auto& line : lines)
    {
        if (line[0] == '.') 
        {
            CompilerDirectives directive = directives[line];
            directive_table[directive](line);
            continue;
        };
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
        // map to function
        std::vector<lc_word_t> instructions = op_table[opcode](tokens);
        for (const auto& instr : instructions)
        {
            memory.push_back(instr);
        }
    }

    // write to file
    std::ofstream out("test.bin", std::ios::binary);
    for (const auto& instr : memory)
    {
        out.write(reinterpret_cast<const char*>(&instr), sizeof(lc_word_t));
    }

}