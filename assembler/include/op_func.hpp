#pragma once

#include <vector>
#include <string>

#include "../../virutal_machine/include/lc_type.hpp"

std::vector<lc_word_t> op_add(std::vector<std::string> tokens);
std::vector<lc_word_t> op_and(std::vector<std::string> tokens);
std::vector<lc_word_t> op_br(std::vector<std::string> tokens);
std::vector<lc_word_t> op_jmp(std::vector<std::string> tokens);
std::vector<lc_word_t> op_jsr(std::vector<std::string> tokens);
std::vector<lc_word_t> op_ld(std::vector<std::string> tokens);
std::vector<lc_word_t> op_st(std::vector<std::string> tokens);
std::vector<lc_word_t> op_ldi(std::vector<std::string> tokens);
std::vector<lc_word_t> op_sti(std::vector<std::string> tokens);
std::vector<lc_word_t> op_ldr(std::vector<std::string> tokens);
std::vector<lc_word_t> op_str(std::vector<std::string> tokens);
std::vector<lc_word_t> op_lea(std::vector<std::string> tokens);
std::vector<lc_word_t> op_not(std::vector<std::string> tokens);
std::vector<lc_word_t> op_or(std::vector<std::string> tokens);
std::vector<lc_word_t> op_ret(std::vector<std::string> tokens);
std::vector<lc_word_t> op_trap(std::vector<std::string> tokens);
