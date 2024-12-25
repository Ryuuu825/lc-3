#include "memory.hpp"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

struct termios original_tio;

void disable_input_buffering() 
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering() 
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

void handle_interrupt(int signal) 
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

void handle_input() 
{
    char c = '\0';
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        switch (c)
        {
            case 'q':
            case 'Q':
            case 4: // Ctrl-D
                printf("\n");
                restore_input_buffering();
                exit(0);
                break;
        }
    }
}

lc_uint_t check_key()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}


lc_word_t Memory::read(lc_uint_t address)
{
    return read_word(address);
}

void Memory::write(lc_uint_t address, lc_word_t value)
{
    write_word(address, value);
}

lc_word_t Memory::read_word(lc_uint_t address)
{
    if (address == mmregister::KBSR)
    {
        if (check_key())
        {
            main_memory[mmregister::KBSR] = 1 << 15;
            main_memory[mmregister::KBDR] = getchar();
        }
        else
        {
            main_memory[mmregister::KBSR] = 0;
        }
    }
    if (address == mmregister::DSR)
    {
        main_memory[mmregister::DSR] = 1 << 15;
    }
    if (address == mmregister::TSR)
    {
        main_memory[mmregister::TSR] = 1 << 15;
    }
    if (address == mmregister::TDR)
    {
        time_t t;
        time(&t);
        main_memory[mmregister::TDR] = t;
    }

    return main_memory[address];
}

lc_byte_t Memory::read_hi_byte(lc_uint_t address)
{
    lc_word_t d = read_word(address);
    return (d >> 8) & 0xFF;
}

lc_byte_t Memory::read_lo_byte(lc_uint_t address)
{
    lc_word_t d = read_word(address);
    return d & 0xFF;
}

void Memory::write_word(lc_uint_t address, lc_word_t value)
{
    if (address == mmregister::DDR && main_memory[mmregister::DSR] == (1 << 15))
    {
        putchar(value);
        fflush(stdout);
        return;
    }
    main_memory[address] = value;
}

void Memory::write_hi_byte(lc_uint_t address, lc_byte_t value)
{
    main_memory[address] = (main_memory[address] & 0xFF) | (value << 8);
}

void Memory::write_lo_byte(lc_uint_t address, lc_byte_t value)
{
    main_memory[address] = (main_memory[address] & 0xFF00) | value;
}