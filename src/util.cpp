// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "util.h"

#include <cstdint>

#include "flags.h"
#include "registers.h"

namespace HCS08 {

const char* FlagToStr(const uint32_t flag_id) {
  switch (flag_id) {
    case Flags::FLAG_C:
      return "C";  // Carry
    case Flags::FLAG_Z:
      return "Z";  // Zero
    case Flags::FLAG_N:
      return "N";  // Negative
    case Flags::FLAG_I:
      return "I";  // Interrupt
    case Flags::FLAG_H:
      return "H";  // Half-carry (from bit 3)
    case Flags::FLAG_V:
      return "V";  // Two's complement overflow
    default:
      return nullptr;
  }
}

const char* RegToStr(const uint8_t reg_id) {
  switch (reg_id) {
    // Accumulator register
    case Registers::HCS08_REG_A:
      return "A";

    // H:X combine to form the high and low parts respectively of a 16-bit index
    // register
    case Registers::HCS08_REG_HX:
      return "H:X";

    // Low byte of index register; register X
    case Registers::HCS08_REG_X:
      return "X";

    // Stack pointer
    case Registers::HCS08_REG_SP:
      return "SP";

    // Program counter
    case Registers::HCS08_REG_PC:
      return "PC";

    // 8-bit condition code register
    case Registers::HCS08_REG_CCR:
      return "CCR";

    default:
      return nullptr;
  }
}
}  // namespace HCS08