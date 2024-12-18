#include <map>
#include <string>
#include <sstream>

#include "directives.hpp"
#include "stringhelper.hpp"

extern lc_word_t address; // logical address
extern std::vector<lc_word_t> memory;
extern std::map<std::string, lc_word_t> symbol_table;

std::map<std::string, CompilerDirectives> directives = {
    {".ORIG", ORIG},
    {".FILL", FILL},
    {".BLKW", BLKW},
    {".STRINGZ", STRINGZ},
    {".END", END}
};

void cd_orig(std::string line)
{
    std::stringstream ss(trim(remove(line, {'.', 'O', 'R', 'I', 'G'})));
    lc_uint_t starting_address = 0x3000;
    ss >> std::hex >> starting_address;
    memory[0] = starting_address;
}

void cd_fill(std::string line)
{
    // symbol table should already have the entry
}

void cd_blkw(std::string line)
{
    std::vector<std::string> tokens = split(line, " ");
    std::stringstream ss(tokens[2]);
    lc_uint_t size = 0;
    ss >> size;
    address += size;
}

// TODO: one char use 1 byte, but one word in lc3 is 2 bytes
void cd_stringz(std::string line)
{
    // calculate size of string
    size_t first_quote = line.find_first_of('"');
    size_t last_quote = line.find_last_of('"');
    std::string str = line.substr(first_quote + 1, last_quote - first_quote - 1);
    address += str.size() + 1; 
}

void cd_end(std::string line)
{
    memory[1] = address; // program size
}

void op_fill(std::string line)
{
    std::vector<std::string> tokens = split(line, " ");
    lc_word_t value = 0;
    if (tokens[1][0] == '#')
    {
        std::stringstream ss(remove(tokens[1], {'#'}));
        ss >> value;
    }
    else
    {
        std::stringstream ss(trim(tokens[1]));
        ss >> std::hex >> value;
    }
    memory.push_back(value);
    address += 1;
}

void op_blkw(std::string line)
{
    std::vector<std::string> tokens = split(line, " ");
    lc_uint_t size = 0;
    if (tokens[1][0] == '#')
    {
        std::stringstream ss(remove(tokens[1], {'#'}));
        ss >> size;
    }
    else
    {
        std::stringstream ss(tokens[1]);
        ss >> std::hex >> size;
    }
    for (lc_uint_t i = 0; i < size; i++)
    {
        memory.push_back(0);
        address += 1;
    }
}

void op_stringz(std::string line)
{
    size_t first_quote = line.find_first_of('"');
    size_t last_quote = line.find_last_of('"');
    std::string str = line.substr(first_quote + 1, last_quote - first_quote - 1);
    for (char c : str)
    {
        memory[address] = c;
        address += 1;
    }
    memory[address] = 0;
    address += 1;
}


std::map<CompilerDirectives, void (*)(std::string)> directive_table = {
    {ORIG, cd_orig},
    {FILL, cd_fill},
    {BLKW, cd_blkw},
    {STRINGZ, cd_stringz},
    {END, cd_end}
};

std::map<CompilerDirectives, void (*)(std::string)> op_directive_table = {
    {ORIG, [](std::string) -> void {}},
    {FILL, op_fill},
    {BLKW, op_blkw},
    {STRINGZ, op_stringz},
    {END, [](std::string) -> void {}}
};