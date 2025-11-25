// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <binaryninjaapi.h>

#include "instructions.h"

namespace BN = BinaryNinja;

namespace HCS08 {
class HCS08Architecture : public BN::Architecture {
 protected:
  static BNRegisterInfo RegisterInfo(uint32_t fullWidthReg, size_t offset,
                                     size_t size, bool zeroExtend = false);

  std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
      INSTRUCTIONS;

 public:
  explicit HCS08Architecture(const std::string& name);
  ~HCS08Architecture() override;

  size_t GetAddressSize() const override;
  size_t GetDefaultIntegerSize() const override;
  BNEndianness GetEndianness() const override;
  uint32_t GetStackPointerRegister() override;
  size_t GetMaxInstructionLength() const override;
  size_t GetInstructionAlignment() const override;
  std::vector<uint32_t> GetAllRegisters() override;
  BNRegisterInfo GetRegisterInfo(uint32_t reg) override;
  std::string GetRegisterName(uint32_t reg) override;
  std::vector<uint32_t> GetAllFlags() override;
  std::string GetFlagName(uint32_t flag) override;
  BNFlagRole GetFlagRole(uint32_t flag, uint32_t semClass) override;
  std::vector<uint32_t> GetFlagsWrittenByFlagWriteType(uint32_t flags) override;
  std::string GetFlagWriteTypeName(uint32_t flags) override;
  std::vector<uint32_t> GetFlagsRequiredForFlagCondition(
      BNLowLevelILFlagCondition cond, uint32_t semClass) override;

  bool GetInstructionInfo(const uint8_t* data, uint64_t addr, size_t maxLen,
                          BN::InstructionInfo& result) override;
  bool GetInstructionText(
      const uint8_t* data, uint64_t addr, size_t& len,
      std::vector<BN::InstructionTextToken>& result) override;
  bool GetInstructionLowLevelIL(const uint8_t* data, uint64_t addr, size_t& len,
                                BN::LowLevelILFunction& il) override;
};
}  // namespace HCS08

#endif  // ARCHITECTURE_H
