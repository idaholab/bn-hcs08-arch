// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef LIFT_H
#define LIFT_H

#include <binaryninjaapi.h>

#include <cstdint>

namespace BN = BinaryNinja;

namespace HCS08 {
bool LiftBranchRel(const uint8_t* data, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BinaryNinja::Architecture* arch);
bool LiftBsetBclrDir(const uint8_t* data, size_t& len,
                     BN::LowLevelILFunction& il);
bool LiftBrsetBrclrDir(const uint8_t* data, uint64_t addr, size_t& len,
                       BN::LowLevelILFunction& il,
                       BinaryNinja::Architecture* arch);
}  // namespace HCS08
#endif  // LIFT_H
