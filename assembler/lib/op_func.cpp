#include <map>
#include <sstream>

#include "opcode.hpp"
#include "op_func.hpp"
#include "stringhelper.hpp"

extern std::map<std::string, lc_word_t> symbol_table;
extern lc_word_t address;

std::vector<lc_word_t> op_add(std::vector<std::string> tokens)
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
    else if (starts_with(tokens[3] , "0x"))
    {
        lc_uint_t imm5;
        std::stringstream ss;
        ss << std::hex << tokens[3];
        ss >> imm5;
        instr |= 0b1 << 5;
        instr |= imm5 & 0b11111;
    }
    else
    {
        std::string sr2 = remove(tokens[3], {'R'});
        lc_uint_t _sr2 = std::stoi(sr2);
        instr |= _sr2;
    }
    return {instr};
}

std::vector<lc_word_t> op_trap(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_TRAP << 12;

    std::stringstream ss;
    ss << std::hex << tokens[1];
    lc_uint_t trapvect8;
    ss >> trapvect8;
    instr |= trapvect8;
    return {instr};
}

std::vector<lc_word_t> op_and(std::vector<std::string> tokens)
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
    else if (starts_with(tokens[3] , "0x"))
    {
        lc_uint_t imm5;
        std::stringstream ss;
        ss << std::hex << tokens[3];
        ss >> imm5;
        instr |= 0b1 << 5;
        instr |= imm5 & 0b11111;
    }
    else
    {
        std::string sr2 = remove(tokens[3], {'R'});
        lc_uint_t _sr2 = std::stoi(sr2);
        instr |= _sr2;
    }
    return {instr};
}

std::vector<lc_word_t> op_br(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_BR << 12;

    lc_uint_t n = 0;
    lc_uint_t z = 0;
    lc_uint_t p = 0;

    for (const auto& token : tokens[0])
    {
        // to uppercase
        char c = std::toupper(token);
        if (c == 'N') n = 1;
        if (c == 'Z') z = 1;
        if (c == 'P') p = 1;
    }

    // unconditional branch
    if (tokens.size() == 2)
    {
        lc_int_t pc_offset9;

        if (starts_with(tokens[1], "#"))
        {
            pc_offset9 = std::stoi(remove(tokens[1], {'#'}));
        } 
        else if (starts_with(tokens[1], "x") || starts_with(tokens[1], "0x"))
        {
            std::stringstream ss;
            ss << std::hex << tokens[1];
            ss >> pc_offset9;
        }
        else {
            // label
            pc_offset9 = symbol_table[tokens[1]] - address - 1;
        }
         
        instr |= (n << 11) | (z << 10) | (p << 9);
        instr |= pc_offset9 & 0x1FF;
        return {instr};
    }
}

std::vector<lc_word_t> op_jmp(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_JMP << 12;

    std::string base_r = remove(tokens[1], {'R'});
    lc_uint_t _base_r = std::stoi(base_r);
    instr |= (0b000) << 9; // N Z P are all 0
    instr |= _base_r << 6;

    return {instr};
}

std::vector<lc_word_t> op_jsr(std::vector<std::string> tokens)
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
        } 
        else if (starts_with(tokens[1], "x") || starts_with(tokens[1], "0x"))
        {
            std::stringstream ss;
            ss << std::hex << tokens[1];
            ss >> pc_offset11;
        }
        else {
            // label
            pc_offset11 = symbol_table[tokens[1]] - address - 1;
            if (pc_offset11 >= 0)
            {
                pc_offset11 += 1;
            }
        }
        instr |= (0b1) << 11;
        instr |= (pc_offset11 - 1) & 0x7FF;
        return {instr};
    }
    // JSRR
    else
    {
        std::string base_r = remove(tokens[1], {'R'});
        lc_uint_t _base_r = std::stoi(base_r);
        instr |= _base_r << 6;
        return {instr};
    }
}

std::vector<lc_word_t> op_ret(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_JMP << 12;
    instr |= 0b000 << 9;
    instr |= 0b111 << 6;
    return {instr};
}

std::vector<lc_word_t> op_not(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_NOT << 12;

    lc_uint_t dr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= dr << 9;

    lc_uint_t sr = std::stoi(remove(tokens[2], {'R'}));
    instr |= sr << 6;

    instr |= 0b111111;
    return {instr};
}

std::vector<lc_word_t> op_ld(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_LD << 12;

    lc_uint_t dr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= dr << 9;

    lc_uint_t pc_offset9 = 0;
    if (starts_with(tokens[2], "#"))
    {
        pc_offset9 = std::stoi(tokens[2].substr(1));
    }
    else if (starts_with(tokens[2], "x") || starts_with(tokens[2], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[2];
        ss >> pc_offset9;
    }
    else {
        // label
        pc_offset9 = symbol_table[tokens[2]] - address - 1;
    }
    instr |= pc_offset9 & 0x1FF;
    return {instr};
}

std::vector<lc_word_t> op_st(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_ST << 12;

    lc_uint_t sr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= sr << 9;

    lc_uint_t pc_offset9 = 0;
    if (starts_with(tokens[2], "#"))
    {
        pc_offset9 = std::stoi(tokens[2].substr(1));
    }
    else if (starts_with(tokens[2], "x") || starts_with(tokens[2], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[2];
        ss >> pc_offset9;
    }
    else {
        // label
        pc_offset9 = symbol_table[tokens[2]] - address - 1;
    }
    instr |= pc_offset9 & 0x1FF;
    return {instr};
}

std::vector<lc_word_t> op_ldi(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_LDI << 12;

    lc_uint_t dr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= dr << 9;

    lc_uint_t pc_offset9 = 0;
    if (starts_with(tokens[2], "#"))
    {
        pc_offset9 = std::stoi(tokens[2].substr(1));
    }
    else if (starts_with(tokens[2], "x") || starts_with(tokens[2], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[2];
        ss >> pc_offset9;
    }
    else {
        // label
        pc_offset9 = symbol_table[tokens[2]] - address - 1;
    }
    instr |= pc_offset9 & 0x1FF;
    return {instr};
}

std::vector<lc_word_t> op_sti(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_STI << 12;

    lc_uint_t sr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= sr << 9;

    lc_uint_t pc_offset9 = 0;
    if (starts_with(tokens[2], "#"))
    {
        pc_offset9 = std::stoi(tokens[2].substr(1));
    }
    else if (starts_with(tokens[2], "x") || starts_with(tokens[2], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[2];
        ss >> pc_offset9;
    }
    else {
        // label
        pc_offset9 = symbol_table[tokens[2]] - address - 1;
    }
    instr |= pc_offset9 & 0x1FF;
    return {instr};
}

std::vector<lc_word_t> op_ldr(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_LDR << 12;

    lc_uint_t dr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= dr << 9;

    lc_uint_t base_r = std::stoi(remove(tokens[2], {'R', ','}));
    instr |= base_r << 6;

    lc_uint_t offset6 = 0;
    if (starts_with(tokens[3], "#"))
    {
        offset6 = std::stoi(tokens[3].substr(1));
    }
    else if (starts_with(tokens[3], "x") || starts_with(tokens[3], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[3];
        ss >> offset6;
    }
    else {
        // label
        offset6 = symbol_table[tokens[3]] - address - 1;
    }
    instr |= offset6 & 0x3F;
    return {instr};
}

std::vector<lc_word_t> op_str(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_STR << 12;

    lc_uint_t sr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= sr << 9;

    lc_uint_t base_r = std::stoi(remove(tokens[2], {'R', ','}));
    instr |= base_r << 6;

    lc_uint_t offset6 = 0;
    if (starts_with(tokens[3], "#"))
    {
        offset6 = std::stoi(tokens[3].substr(1));
    }
    else if (starts_with(tokens[3], "x") || starts_with(tokens[3], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[3];
        ss >> offset6;
    }
    else {
        // label
        offset6 = symbol_table[tokens[3]] - address - 1;
    }
    instr |= offset6 & 0x3F;
    return {instr};
}

std::vector<lc_word_t> op_lea(std::vector<std::string> tokens)
{
    lc_word_t instr = 0;
    instr |= OP_LEA << 12;

    lc_uint_t dr = std::stoi(remove(tokens[1], {'R', ','}));
    instr |= dr << 9;

    lc_uint_t pc_offset9 = 0;
    if (starts_with(tokens[2], "#"))
    {
        pc_offset9 = std::stoi(tokens[2].substr(1));
    }
    else if (starts_with(tokens[2], "x") || starts_with(tokens[2], "0x"))
    {
        std::stringstream ss;
        ss << std::hex << tokens[2];
        ss >> pc_offset9;
    }
    else {
        // label
        pc_offset9 = symbol_table[tokens[2]] - address - 1;
    }
    instr |= pc_offset9 & 0x1FF;
    return {instr};
}

std::map<Opcode, std::vector<lc_word_t> (*)(std::vector<std::string>)> op_table = {
    {OP_ADD, op_add},
    {OP_AND, op_and},
    {OP_BR, op_br},
    {OP_JMP, op_jmp},
    {OP_JSR, op_jsr},
    {OP_JSRR, op_jsr},
    {OP_LD, op_ld},
    {OP_LDI, op_ldi},
    {OP_LDR, op_ldr},
    {OP_LEA, op_lea},
    {OP_NOT, op_not},
    {OP_ST, op_st},
    {OP_STI, op_sti},
    {OP_STR, op_str},
    {OP_TRAP, op_trap},
    {OP_RET, op_ret},
};