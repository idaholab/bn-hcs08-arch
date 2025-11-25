// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef SIZES_H
#define SIZES_H

namespace HCS08::Sizes {
// Data sizes, based on 16-bit Sizes::WORD size (16-bit addressing)
constexpr size_t BYTE = 1;
constexpr size_t WORD = 2;

// Instruction sizes
constexpr size_t INSTRUCTION_1_BYTE = 1;
constexpr size_t INSTRUCTION_2_BYTES = 2;
constexpr size_t INSTRUCTION_3_BYTES = 3;
constexpr size_t INSTRUCTION_4_BYTES = 4;

}  // namespace HCS08::Sizes

#endif  // SIZES_H
