#pragma once

enum mmregister {
    KBSR = 0xFE00,
    KBDR = 0xFE02,
    DSR = 0xFE04,
    DDR = 0xFE06,
    TSR = 0xFE08, // Timer status register
    TDR = 0xFE0A, // Timer data register
    MCR = 0xFFFE // Machine control register
};