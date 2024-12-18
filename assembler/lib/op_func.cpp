#include <map>
#include <sstream>

#include "opcode.hpp"
#include "op_func.hpp"
#include "stringhelper.hpp"

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