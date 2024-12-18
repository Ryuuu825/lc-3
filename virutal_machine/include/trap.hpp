#pragma once

#include <cstdio>
#include "lc_type.hpp"
#include "register.hpp"
#include "memory.hpp"

void trap_getc();
void trap_out();
void trap_halt();

typedef void (*trap_func)(void);
