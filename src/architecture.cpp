// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "architecture.h"

#include <binaryninjaapi.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "flags.h"
#include "instructions.h"
#include "registers.h"
#include "sizes.h"
#include "text.h"
#include "util.h"

namespace BN = BinaryNinja;

namespace HCS08 {
HCS08Architecture::HCS08Architecture(const std::string& name)
    : Architecture(name) {
  // The INSTRUCTIONS map contains a map of opcode -> Instruction objects for
  // fast lookup in Info/Text/Lift methods
  INSTRUCTIONS = GenerateInstructionMap();
}

HCS08Architecture::~HCS08Architecture() { INSTRUCTIONS.clear(); }

BNRegisterInfo HCS08Architecture::RegisterInfo(const uint32_t fullWidthReg,
                                               const size_t offset,
                                               const size_t size,
                                               const bool zeroExtend) {
  BNRegisterInfo result;

  result.fullWidthRegister = fullWidthReg;
  result.offset = offset;
  result.size = size;
  result.extend = zeroExtend ? ZeroExtendToFullWidth : NoExtend;

  return result;
}

[[nodiscard]] size_t HCS08Architecture::GetAddressSize() const {
  return Sizes::WORD;  // 16-bit
}

[[nodiscard]] size_t HCS08Architecture::GetDefaultIntegerSize() const {
  return Sizes::BYTE;
}

[[nodiscard]] BNEndianness HCS08Architecture::GetEndianness() const {
  return BigEndian;
}

[[nodiscard]] uint32_t HCS08Architecture::GetStackPointerRegister() {
  return Registers::HCS08_REG_SP;
}

[[nodiscard]] size_t HCS08Architecture::GetMaxInstructionLength() const {
  return Sizes::INSTRUCTION_4_BYTES;
}

[[nodiscard]] size_t HCS08Architecture::GetInstructionAlignment() const {
  return Sizes::BYTE;
}

[[nodiscard]] std::vector<uint32_t> HCS08Architecture::GetAllRegisters() {
  return std::vector<uint32_t>{
      Registers::HCS08_REG_A,  Registers::HCS08_REG_HX,
      Registers::HCS08_REG_X,  Registers::HCS08_REG_SP,
      Registers::HCS08_REG_PC, Registers::HCS08_REG_CCR};
}

[[nodiscard]] BNRegisterInfo HCS08Architecture::GetRegisterInfo(uint32_t rid) {
  switch (rid) {
    case Registers::HCS08_REG_A:
      return RegisterInfo(rid, 0,
                          Sizes::BYTE);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    case Registers::HCS08_REG_HX:
      return RegisterInfo(rid, 0,
                          Sizes::WORD);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    case Registers::HCS08_REG_X:         // Low byte of HCS08_REG_HX
      return RegisterInfo(Registers::HCS08_REG_HX, 0,
                          Sizes::BYTE);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    case Registers::HCS08_REG_SP:
      return RegisterInfo(rid, 0,
                          Sizes::WORD);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    case Registers::HCS08_REG_PC:
      return RegisterInfo(rid, 0,
                          Sizes::WORD);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    case Registers::HCS08_REG_CCR:
      return RegisterInfo(rid, 0,
                          Sizes::BYTE);  // struct contains: full width reg,
                                         // offset (for sub-registers), size
    default:
      return RegisterInfo(0, 0, 0);
  }
}

[[nodiscard]] std::string HCS08Architecture::GetRegisterName(uint32_t rid) {
  /* Note: register name mapping function is defined in util.cpp */
  const char* result = RegToStr(rid);
  if (result == nullptr) {
    result = "GetRegisterName: INVALID_REG_ID";
  }
  return result;
}

[[nodiscard]] std::vector<uint32_t> HCS08Architecture::GetAllFlags() {
  return std::vector<uint32_t>{
      Flags::FLAG_C,  // Carry
      Flags::FLAG_Z,  // Zero
      Flags::FLAG_N,  // Negative
      Flags::FLAG_I,  // Interrupt
      Flags::FLAG_H,  // Half-carry (from bit 3)
      Flags::FLAG_V   // Two's complement overflow
  };
}

[[nodiscard]] std::string HCS08Architecture::GetFlagName(uint32_t flag) {
  /* Note: flag name mapping function is defined in util.cpp */
  const char* result = FlagToStr(flag);
  if (result == nullptr) {
    result = "GetFlagName: INVALID_FLAG_ID";
  }
  return result;
}

[[nodiscard]] BNFlagRole HCS08Architecture::GetFlagRole(uint32_t flag,
                                                        uint32_t semClass) {
  switch (flag) {
    case Flags::FLAG_C:
      return CarryFlagRole;
    case Flags::FLAG_Z:
      return ZeroFlagRole;
    case Flags::FLAG_N:
      return NegativeSignFlagRole;
    case Flags::FLAG_V:
      return OverflowFlagRole;
    case Flags::FLAG_I:  // Interrupt flag
    case Flags::FLAG_H:  // Half-carry flag
    default:
      return SpecialFlagRole;  // TODO: is this correct?
  }
}

[[nodiscard]] std::vector<uint32_t>
HCS08Architecture::GetFlagsWrittenByFlagWriteType(uint32_t flags) {
  switch (flags) {
    case Flags::FLAGS_Z:
      return std::vector<uint32_t>{Flags::FLAG_Z};
    case Flags::FLAGS_C_Z:
      return std::vector<uint32_t>{Flags::FLAG_C, Flags::FLAG_Z};
    case Flags::FLAGS_Z_N_V:
      return std::vector<uint32_t>{Flags::FLAG_Z, Flags::FLAG_N, Flags::FLAG_V};
    case Flags::FLAGS_C_Z_N_V:
      return std::vector<uint32_t>{Flags::FLAG_C, Flags::FLAG_Z, Flags::FLAG_N,
                                   Flags::FLAG_V};
    case Flags::FLAGS_C_Z_N_H_V:
      return std::vector<uint32_t>{Flags::FLAG_C, Flags::FLAG_Z, Flags::FLAG_N,
                                   Flags::FLAG_H, Flags::FLAG_V};
    default:
      return std::vector<uint32_t>{};
  }
}

[[nodiscard]] std::string HCS08Architecture::GetFlagWriteTypeName(
    uint32_t flags) {
  switch (flags) {
    case Flags::FLAGS_Z:
      return "z";
    case Flags::FLAGS_C_Z:
      return "cz";
    case Flags::FLAGS_Z_N_V:
      return "znv";
    case Flags::FLAGS_C_Z_N_V:
      return "cznv";
    case Flags::FLAGS_C_Z_N_H_V:
      return "cznhv";
    default:
      return "GetFlagWriteTypeName: flag write type undefined";
  }
}

[[nodiscard]] std::vector<uint32_t>
HCS08Architecture::GetFlagsRequiredForFlagCondition(
    BNLowLevelILFlagCondition cond, uint32_t semClass) {
  switch (cond) {
    case LLFC_E:
    case LLFC_NE:
      return std::vector<uint32_t>{Flags::FLAG_Z};
    case LLFC_NEG:
    case LLFC_POS:
      return std::vector<uint32_t>{Flags::FLAG_N};
    case LLFC_O:
    case LLFC_NO:
      return std::vector<uint32_t>{Flags::FLAG_V};
    case LLFC_ULT:
    case LLFC_UGE:
      return std::vector<uint32_t>{Flags::FLAG_C};
    case LLFC_ULE:
    case LLFC_UGT:
      return std::vector<uint32_t>{Flags::FLAG_Z, Flags::FLAG_C};
    case LLFC_SGE:
    case LLFC_SLT:
      return std::vector<uint32_t>{Flags::FLAG_N, Flags::FLAG_V};
    case LLFC_SLE:
    case LLFC_SGT:
      return std::vector<uint32_t>{Flags::FLAG_Z, Flags::FLAG_N, Flags::FLAG_V};
    case LLFC_FE:
    case LLFC_FNE:
    case LLFC_FLT:
    case LLFC_FLE:
    case LLFC_FGE:
    case LLFC_FGT:
    case LLFC_FO:
    case LLFC_FUO:
    default:
      return std::vector<uint32_t>{};
  }
}

/**
 * GetInstructionInfo parses opcode; determines instruction length and
 * branching behavior
 * @param data   - pointer to the next byte in the binary that is being analyzed
 * @param addr   - current address value held by the program counter
 * @param maxLen - max instruction length
 * @param result - InstructionInfo object to be populated with instruction
 * length and branch behavior
 * @return bool indicating whether a valid instruction was parsed
 */
bool HCS08Architecture::GetInstructionInfo(const uint8_t* data,
                                           const uint64_t addr,
                                           const size_t maxLen,
                                           BN::InstructionInfo& result) {
  if (const auto i = DecodeInstruction(data, INSTRUCTIONS)) {
    return i->get()->Info(data, addr, result);
  }
  return false;
}

/**
 * GetInstructionText parses opcode; generates disassembly text
 * @param data   - pointer to the next byte in the binary that is being analyzed
 * @param addr   - current address value held by the program counter
 * @param len - length of the instruction (set by this function call)
 * @param result - InstructionTextToken vector to be populated with instruction
 * text
 * @return bool indicating whether a valid instruction was parsed
 */
bool HCS08Architecture::GetInstructionText(
    const uint8_t* data, uint64_t addr, size_t& len,
    std::vector<BN::InstructionTextToken>& result) {
  if (const auto i = DecodeInstruction(data, INSTRUCTIONS)) {
    len = i->get()->GetLength();
    return i->get()->Text(data, addr, len, result);
  }
  return false;
}

/**
 * GetInstructionLowLevelIL parses opcode; generates binja LLIL
 * @param data   - pointer to the next byte in the binary that is being analyzed
 * @param addr   - current address value held by the program counter
 * @param len - length of the instruction (set by this function call)
 * @param il - LowLevelILFunction object used to generate the LLIL
 * @return bool indicating whether a valid instruction was parsed
 */
bool HCS08Architecture::GetInstructionLowLevelIL(const uint8_t* data,
                                                 uint64_t addr, size_t& len,
                                                 BN::LowLevelILFunction& il) {
  if (const auto i = DecodeInstruction(data, INSTRUCTIONS)) {
    return i->get()->Lift(data, addr, len, il, this);
  }
  return false;
}

}  // namespace HCS08
extern "C" {
BN_DECLARE_CORE_ABI_VERSION
BINARYNINJAPLUGIN bool CorePluginInit() {
  BN::Architecture* hcs08 = new HCS08::HCS08Architecture("hcs08");
  BN::Architecture::Register(hcs08);

  // TODO: define and add a calling convention

  return true;
}
}