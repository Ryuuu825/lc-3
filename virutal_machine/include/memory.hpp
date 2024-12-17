#pragma once

#include <array>
#include <chrono>
#include <thread>

#include "lc_type.hpp"

// 2^16 entries => 2^16 * 2 bytes = 2^16 * 2 bytes = 2^17 bytes = 128KB
#define MEMORY_MAX 65536 
#define MEMORY_LATENCY 1

#define L1_CACHE_SIZE 16
#define L1_CACHE_LATENCY 1

// this system uses big-endian and byte-addressable memory
// please use read / read_word / write / write_word to access memory in system
// since no cache is in lc-3, we only implement one level cache
struct Memory 
{
public:

    inline lc_word_t read(lc_int_t address)
    {
        return read_word(address);
    }

    inline void write(lc_int_t address, lc_word_t value)
    {
        write_word(address, value);
    }

    inline lc_word_t read_word(lc_int_t address)
    {
        return main_memory[address];
    }

    inline lc_byte_t read_hi_byte(lc_int_t address)
    {
        lc_word_t d = read_word(address);
        return (d >> 8) & 0xFF;
    }

    inline lc_byte_t read_lo_byte(lc_int_t address)
    {
        lc_word_t d = read_word(address);
        return d & 0xFF;
    }

    inline void write_word(lc_int_t address, lc_word_t value)
    {
        main_memory[address] = value;
    }

    inline void write_hi_byte(lc_int_t address, lc_byte_t value)
    {
        main_memory[address] = (main_memory[address] & 0xFF) | (value << 8);
    }

    inline void write_lo_byte(lc_int_t address, lc_byte_t value)
    {
        main_memory[address] = (main_memory[address] & 0xFF00) | value;
    }

    std::array<lc_word_t, MEMORY_MAX> main_memory = {};

};
