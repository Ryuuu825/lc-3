#pragma once

#include <vector>
#include <string>

#include "../../virutal_machine/include/lc_type.hpp"

lc_word_t op_add(std::vector<std::string> tokens);
lc_word_t op_trap(std::vector<std::string> tokens);
lc_word_t op_and(std::vector<std::string> tokens);
lc_word_t op_br(std::vector<std::string> tokens);
lc_word_t op_jmp(std::vector<std::string> tokens);
lc_word_t op_jsr(std::vector<std::string> tokens);
lc_word_t op_ret(std::vector<std::string> tokens);
