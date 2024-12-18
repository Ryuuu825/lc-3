#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector> 
#include <map>

#include "../virutal_machine/include/lc_type.hpp"

enum Opcode
{
    OP_BR = 0b0, // Branch
    OP_ADD, // Add
    OP_LD, // Load
    OP_ST, // Store
    OP_JSR, // Jump Register
    OP_AND, // Bitwise AND
    OP_LDR, // Load Register
    OP_STR, // Store Register
    OP_RTI, // Return from Interrupt
    OP_NOT, // Bitwise NOT
    OP_LDI, // Load Indirect
    OP_STI, // Store Indirect
    OP_JMP, // Jump
    OP_RES, // Reserved (unused)
    OP_LEA, // Load Effective Address
    OP_TRAP // Execute Trap

    ,
    OP_BRN, // branch if negative
    OP_BRZ, // branch if zero
    OP_BRP, // branch if positive
    OP_BRNZ, // branch if not zero
    OP_BRNP, // branch if not positive
    OP_BRZP, // branch if not zero

    // pseudo opcodes (to be implemented)
    OP_BREQ, // branch if equal 
    OP_BRNE, // branch if not equal
    OP_BRGT, // branch if greater than
    OP_BRGE, // branch if greater than or equal
    OP_BRLT, // branch if less than
    OP_BRLE, // branch if less than or equals
    OP_BREQZ, // branch if equal to zero
    OP_BRNEZ, // branch if not equal to zero
    OP_RET, // return

    OP_JSRR, // Jump Subroutine Register
};

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string remove(const std::string& str, const std::vector<char>& delim)
{
    std::string result = str;
    for (const auto& c : delim)
    {
        result.erase(std::remove(result.begin(), result.end(), c), result.end());
    }
    return result;
}

bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.find(prefix) == 0;
}

std::string remove_after(const std::string& str, const char delim)
{
    size_t pos = str.find(delim);
    if (pos == std::string::npos) return str;
    return str.substr(0, pos);
}

std::map<std::string, Opcode> opcodes = {
    {"ADD", OP_ADD},
    {"AND", OP_AND},
    {"BR", OP_BR},
    {"JMP", OP_JMP},
    {"JSR", OP_JSR},
    {"JSRR", OP_JSRR},
    {"LD", OP_LD},
    {"LDI", OP_LDI},
    {"LDR", OP_LDR},
    {"LEA", OP_LEA},
    {"NOT", OP_NOT},
    {"ST", OP_ST},
    {"STI", OP_STI},
    {"STR", OP_STR},
    {"TRAP", OP_TRAP},
    {"BRN", OP_BRN},
    {"BRZ", OP_BRZ},
    {"BRP", OP_BRP},
    {"BRNZ", OP_BRNZ},
    {"BRNP", OP_BRNP},
    {"BRZP", OP_BRZP},
    {"BREQ", OP_BREQ},
    {"BRNE", OP_BRNE},
    {"BRGT", OP_BRGT},
    {"BRGE", OP_BRGE},
    {"BRLT", OP_BRLT},
    {"BRLE", OP_BRLE},
    {"BREQZ", OP_BREQZ},
    {"BRNEZ", OP_BRNEZ},
    {"RET", OP_RET},
};

lc_word_t op_add(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_ADD << 12;

    std::string dr = remove(tokens[1], {'R', ','});
    lc_uint_t _dr = std::stoi(dr);
    instr |= _dr << 9;

    std::string sr1 = remove(tokens[2], {'R', ','});
    lc_uint_t _sr1 = std::stoi(sr1);
    instr |= _sr1 << 6;

    // start with #
    if (starts_with(tokens[3], "#"))
    {
        lc_uint_t imm5 = std::stoi(tokens[3].substr(1));
        instr |= 0b1 << 5;
        instr |= imm5 & 0b11111;
    }
    else
    {
        std::string sr2 = remove(tokens[3], {'R'});
        lc_uint_t _sr2 = std::stoi(sr2);
        instr |= _sr2;
    }
    return instr;
}

lc_word_t op_trap(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_TRAP << 12;

    std::stringstream ss;
    ss << std::hex << tokens[1];
    lc_uint_t trapvect8;
    ss >> trapvect8;
    instr |= trapvect8;
    return instr;
}

lc_word_t op_and(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_AND << 12;

    std::string dr = remove(tokens[1], {'R', ','});
    lc_uint_t _dr = std::stoi(dr);
    instr |= _dr << 9;

    std::string sr1 = remove(tokens[2], {'R', ','});
    lc_uint_t _sr1 = std::stoi(sr1);
    instr |= _sr1 << 6;

    // start with #
    if (starts_with(tokens[3], "#"))
    {
        lc_uint_t imm5 = std::stoi(tokens[3].substr(1));
        instr |= 0b1 << 5;
        instr |= imm5 & 0b11111;
    }
    else
    {
        std::string sr2 = remove(tokens[3], {'R'});
        lc_uint_t _sr2 = std::stoi(sr2);
        instr |= _sr2;
    }
    return instr;
}

lc_word_t op_br(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_BR << 12;

    // unconditional branch
    if (tokens.size() == 2)
    {
        lc_uint_t pc_offset9;

         if (starts_with(tokens[1], "#"))
         {
            pc_offset9 = std::stoi(remove(tokens[1], {'#'}));
         } else 
         {
            std::stringstream ss;
            ss << std::hex << tokens[1];
            ss >> pc_offset9;
         }
         
        instr |= (0b000) << 9; // N Z P are all 0
        instr |= pc_offset9 & 0x1FF;
        return instr;
    }
}

lc_word_t op_jmp(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_JMP << 12;

    std::string base_r = remove(tokens[1], {'R'});
    lc_uint_t _base_r = std::stoi(base_r);
    instr |= (0b000) << 9; // N Z P are all 0
    instr |= _base_r << 6;

    return instr;
}

lc_word_t op_jsr(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_JSR << 12;

    // JSR
    if (tokens.size() == 2)
    {
        lc_uint_t pc_offset11;
        if (starts_with(tokens[1], "#"))
        {
            pc_offset11 = std::stoi(remove(tokens[1], {'#'}));
        } else 
        {
            std::stringstream ss;
            ss << std::hex << tokens[1];
            ss >> pc_offset11;
        }
        instr |= (0b1) << 11;
        instr |= (pc_offset11 - 1) & 0x7FF;
        return instr;
    }
    // JSRR
    else
    {
        std::string base_r = remove(tokens[1], {'R'});
        lc_uint_t _base_r = std::stoi(base_r);
        instr |= _base_r << 6;
        return instr;
    }
}

lc_word_t op_ret(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_JMP << 12;
    instr |= 0b000 << 9;
    instr |= 0b111 << 6;
    return instr;
}


std::map<Opcode, lc_word_t (*)(std::vector<std::string>)> op_table = {
    {OP_ADD, op_add},
    {OP_AND, op_and},
    {OP_BR, op_br},
    {OP_JMP, op_jmp},
    {OP_JSR, op_jsr},
    {OP_JSRR, op_jsr},
    // {OP_LD, op_ld},
    // {OP_LDI, op_ldi},
    // {OP_LDR, op_ldr},
    // {OP_LEA, op_lea},
    // {OP_NOT, op_not},
    // {OP_ST, op_st},
    // {OP_STI, op_sti},
    // {OP_STR, op_str},
    {OP_TRAP, op_trap},
    {OP_RET, op_ret}
};

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

    // first pass
    std::map<std::string, lc_word_t> symbol_table;
    lc_word_t address = 0; // logical address
    for (const auto& line : lines)
    {
        if (line[0] == '.') continue;
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
    std::vector<lc_word_t> memory;
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