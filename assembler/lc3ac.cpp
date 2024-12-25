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
extern std::map<CompilerDirectives, void (*)(std::string)> op_directive_table;

std::vector<lc_word_t> memory;
std::map<std::string, lc_word_t> symbol_table;
lc_word_t address = 0; // logical address
lc_word_t starting_location_counter = 0; // the first instruction address

int main(int argc, char** argv)
{
    std::ifstream file("test.asm");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        if (trim(line)[0] == ';') continue;
        if (trim(line)[0] == '\n') continue;
        if (trim(line)[0] == '@') continue;
        if (trim(line)[0] == ' ') continue; // empty line with spaces
        lines.push_back(trim(line));
    }

    memory.reserve(lines.size() * 2);
    memory.push_back(0); // starting address of the program

    // first pass
    for (auto& line : lines)
    {
        if (starts_with(line, "."))
        {
            CompilerDirectives directive = directives[line];
            directive_table[directive](line);
            continue;
        };

        std::vector<std::string> tokens = split(remove_after(line, ';'), " ");
        if (tokens[0].back() == ':') // label
        {
            symbol_table[ remove(tokens[0], {':'}) ] = address;
            if (tokens[1].find(".") == 0) {
                // parse directive
                CompilerDirectives directive = directives[trim(tokens[1])];
                directive_table[directive](line);
            }
            // remove label
            line = line.substr(tokens[0].size() + 1);
        } 
        // increment address
        address += 1;
    }

    address = 0;

    // second pass
    for (const auto& line : lines)
    {
        std::vector<std::string> tokens = split(remove_after(line, ';'), " ");
        if (line[0] == '.') {
            CompilerDirectives directive = directives[tokens[0]];
            op_directive_table[directive](line);
            continue;
        };
        // parse instruction
        std::string op = tokens[0];
        // make the op code all uppercase
        std::transform(op.begin(), op.end(), op.begin(), ::toupper);
        Opcode opcode = opcodes[op];

        // map to function
        std::vector<lc_word_t> instructions = op_table[opcode](tokens);
        for (const auto& instr : instructions)
        {
            memory.push_back(instr);
        }
        address += 1;
    }

    // write to file
    std::ofstream out("test.bin", std::ios::binary);
    for (const auto& instr : memory)
    {
        out.write(reinterpret_cast<const char*>(&instr), sizeof(lc_word_t));
    }

}