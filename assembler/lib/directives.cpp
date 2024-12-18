#include <map>
#include <string>

#include "directives.hpp"

extern std::vector<lc_word_t> memory;

std::map<std::string, CompilerDirectives> directives = {
    {".ORIG", ORIG},
    {".FILL", FILL},
    {".BLKW", BLKW},
    {".STRINGZ", STRINGZ},
    {".END", END}
};

void cd_orig(std::string line)
{
}

void cd_fill(std::string line)
{
}

void cd_blkw(std::string line)
{
}

void cd_stringz(std::string line)
{
}

void cd_end(std::string line)
{
}


std::map<CompilerDirectives, void (*)(std::string)> directive_table = {
    {ORIG, cd_orig},
    {FILL, cd_fill},
    {BLKW, cd_blkw},
    {STRINGZ, cd_stringz},
    {END, cd_end}
};