// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef INFO_H
#define INFO_H
#include <binaryninjaapi.h>

#include <cstdint>

namespace BN = BinaryNinja;

namespace HCS08 {
bool InfoBranch(const uint8_t* opcode, uint64_t addr,
                BN::InstructionInfo& result);
bool InfoBrsetBrclr(int8_t relOffset, uint64_t addr,
                    BN::InstructionInfo& result);
bool InfoCbeqDirImm(int8_t relOffset, uint64_t addr,
                    BN::InstructionInfo& result);
bool InfoCbeqIxPlus(int8_t relOffset, uint64_t addr,
                    BN::InstructionInfo& result);
bool InfoDbnzDirIx1(int8_t relOffset, uint64_t addr,
                    BN::InstructionInfo& result);
bool InfoDbnzInhIx(int8_t relOffset, uint64_t addr,
                   BN::InstructionInfo& result);
}  // namespace HCS08

#endif  // INFO_H
