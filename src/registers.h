// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef HCS08_REGISTERS_H
#define HCS08_REGISTERS_H

namespace HCS08::Registers {
/* 8-bit small general purpose registers */
constexpr uint8_t HCS08_REG_A = 0;  // Accumulator register

constexpr uint8_t HCS08_REG_HX =
    1;  // H:X combine to form the high and low parts respectively of a 16-bit
// index register
constexpr uint8_t HCS08_REG_X = 2;

/* 16-bit program registers */
constexpr uint8_t HCS08_REG_SP = 3;  // Stack pointer
constexpr uint8_t HCS08_REG_PC = 4;  // Program counter

/* 8-bit condition code register */
constexpr uint8_t HCS08_REG_CCR = 5;
}  // namespace HCS08::Registers

#endif  // HCS08_REGISTERS_H
