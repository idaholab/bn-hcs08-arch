// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "opcodes.h"

#include <vector>

namespace HCS08::Opcodes {
std::vector<int> GetAllOpcodes() {
  std::vector<int> opcodes;
  opcodes.reserve(NAMES.size());
  for (const auto& [opcode, _] : NAMES) {
    opcodes.push_back(opcode);
  }
  return opcodes;
}

// Construct a list of all non-branching instructions
std::vector<int> GetNonBranchingOpcodes() {
  std::vector<int> opcodes;
  opcodes.reserve(NAMES.size());
  for (const auto& [opcode, _] : NAMES) {
    if (BRANCHING.find(opcode) == BRANCHING.end()) {
      opcodes.push_back(opcode);
    }
  }
  return opcodes;
}
}  // namespace HCS08::Opcodes
