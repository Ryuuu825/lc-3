#pragma once

#include <array>
#include <chrono>
#include <thread>

#include "lc_type.hpp"
#include "mmregister.hpp"

// 2^16 entries => 2^16 * 2 bytes = 2^16 * 2 bytes = 2^17 bytes = 128KB
#define MEMORY_MAX 65535 + 1 
#define MEMORY_LATENCY 1

void disable_input_buffering();
void restore_input_buffering();
void handle_interrupt(int signal);
void handle_input();
lc_uint_t check_key();

struct Memory 
{
    lc_word_t read(lc_uint_t address);
    void write(lc_uint_t address, lc_word_t value);
    lc_word_t read_word(lc_uint_t address);
    lc_byte_t read_hi_byte(lc_uint_t address);
    lc_byte_t read_lo_byte(lc_uint_t address);
    void write_word(lc_uint_t address, lc_word_t value);
    void write_hi_byte(lc_uint_t address, lc_byte_t value);
    void write_lo_byte(lc_uint_t address, lc_byte_t value);
    std::array<lc_word_t, MEMORY_MAX> main_memory = {};
};
