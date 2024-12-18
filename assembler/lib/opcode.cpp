#include <map>
#include <string>

#include "opcode.hpp"
#include "op_func.hpp"

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
