// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef HCS08_INSTRUCTIONS_H
#define HCS08_INSTRUCTIONS_H

#include <binaryninjaapi.h>
#include <stdint.h>

#include "sizes.h"

namespace BN = BinaryNinja;

namespace HCS08 {
/*
 * Abstract instruction class that all instructions must use.
 * This allows the return type of the decoder function (DecodeInstruction) to
 * be generic.
 */
class Instruction {
 public:
  virtual ~Instruction() = default;
  explicit Instruction();

  virtual bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) = 0;

  virtual bool Info(const uint8_t* opcode, uint64_t addr,
                    BN::InstructionInfo& result) = 0;

  virtual bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                    BN::LowLevelILFunction& il, BN::Architecture* arch) = 0;

  virtual size_t GetLength() = 0;
};

/*
 * Subclasses for variable length instructions
 */
class Instruction1Byte : public Instruction {
 public:
  static constexpr size_t length = Sizes::INSTRUCTION_1_BYTE;
  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;
  size_t GetLength() override;
};

class Instruction2Byte : public Instruction {
 public:
  static constexpr size_t length = Sizes::INSTRUCTION_2_BYTES;
  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;
  size_t GetLength() override;
};

class Instruction3Byte : public Instruction {
 public:
  static constexpr size_t length = Sizes::INSTRUCTION_3_BYTES;
  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;
  size_t GetLength() override;
};

class Instruction4Byte : public Instruction {
 public:
  static constexpr size_t length = Sizes::INSTRUCTION_4_BYTES;
  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;
  size_t GetLength() override;
};

std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
GenerateInstructionMap();
std::optional<std::shared_ptr<Instruction>> DecodeInstruction(
    const uint8_t* opcode,
    std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
        instr_map);

/*
 * Concrete classes for CPU instructions that should be returned from
 * DecodeInstruction. Note that all of these will implement the Text and Lift
 * methods, but only branching instructions need to implement Info (the rest can
 * rely on the default Info methods).
 */
class AdcImm final : public Instruction2Byte {
 public:
  explicit AdcImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcDir final : public Instruction2Byte {
 public:
  explicit AdcDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcExt final : public Instruction3Byte {
 public:
  explicit AdcExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcIx2 final : public Instruction3Byte {
 public:
  explicit AdcIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcIx1 final : public Instruction2Byte {
 public:
  explicit AdcIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcIx final : public Instruction1Byte {
 public:
  explicit AdcIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcSp2 final : public Instruction4Byte {
 public:
  explicit AdcSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AdcSp1 final : public Instruction3Byte {
 public:
  explicit AdcSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddImm final : public Instruction2Byte {
 public:
  explicit AddImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddDir final : public Instruction2Byte {
 public:
  explicit AddDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddExt final : public Instruction3Byte {
 public:
  explicit AddExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddIx2 final : public Instruction3Byte {
 public:
  explicit AddIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddIx1 final : public Instruction2Byte {
 public:
  explicit AddIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddIx final : public Instruction1Byte {
 public:
  explicit AddIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddSp2 final : public Instruction4Byte {
 public:
  explicit AddSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AddSp1 final : public Instruction3Byte {
 public:
  explicit AddSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AisImm final : public Instruction2Byte {
 public:
  explicit AisImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AixImm final : public Instruction2Byte {
 public:
  explicit AixImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndImm final : public Instruction2Byte {
 public:
  explicit AndImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndDir final : public Instruction2Byte {
 public:
  explicit AndDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndExt final : public Instruction3Byte {
 public:
  explicit AndExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndIx2 final : public Instruction3Byte {
 public:
  explicit AndIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndIx1 final : public Instruction2Byte {
 public:
  explicit AndIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndIx final : public Instruction1Byte {
 public:
  explicit AndIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndSp2 final : public Instruction4Byte {
 public:
  explicit AndSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AndSp1 final : public Instruction3Byte {
 public:
  explicit AndSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsrDir final : public Instruction2Byte {
 public:
  explicit AsrDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsraInh final : public Instruction1Byte {
 public:
  explicit AsraInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsrxInh final : public Instruction1Byte {
 public:
  explicit AsrxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsrIx1 final : public Instruction2Byte {
 public:
  explicit AsrIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsrIx final : public Instruction1Byte {
 public:
  explicit AsrIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class AsrSp1 final : public Instruction3Byte {
 public:
  explicit AsrSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BccBhsRel final : public Instruction2Byte {
 public:
  explicit BccBhsRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB0 final : public Instruction2Byte {
 public:
  explicit BclrDirB0();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB1 final : public Instruction2Byte {
 public:
  explicit BclrDirB1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB2 final : public Instruction2Byte {
 public:
  explicit BclrDirB2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB3 final : public Instruction2Byte {
 public:
  explicit BclrDirB3();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB4 final : public Instruction2Byte {
 public:
  explicit BclrDirB4();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB5 final : public Instruction2Byte {
 public:
  explicit BclrDirB5();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB6 final : public Instruction2Byte {
 public:
  explicit BclrDirB6();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BclrDirB7 final : public Instruction2Byte {
 public:
  explicit BclrDirB7();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BcsBloRel final : public Instruction2Byte {
 public:
  explicit BcsBloRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BeqRel final : public Instruction2Byte {
 public:
  explicit BeqRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BgeRel final : public Instruction2Byte {
 public:
  explicit BgeRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BgndInh final : public Instruction1Byte {
 public:
  explicit BgndInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BgtRel final : public Instruction2Byte {
 public:
  explicit BgtRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BhccRel final : public Instruction2Byte {
 public:
  explicit BhccRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BhcsRel final : public Instruction2Byte {
 public:
  explicit BhcsRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BhiRel final : public Instruction2Byte {
 public:
  explicit BhiRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BihRel final : public Instruction2Byte {
 public:
  explicit BihRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BilRel final : public Instruction2Byte {
 public:
  explicit BilRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitImm final : public Instruction2Byte {
 public:
  explicit BitImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitDir final : public Instruction2Byte {
 public:
  explicit BitDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitExt final : public Instruction3Byte {
 public:
  explicit BitExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitIx2 final : public Instruction3Byte {
 public:
  explicit BitIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitIx1 final : public Instruction2Byte {
 public:
  explicit BitIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitIx final : public Instruction1Byte {
 public:
  explicit BitIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitSp2 final : public Instruction4Byte {
 public:
  explicit BitSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BitSp1 final : public Instruction3Byte {
 public:
  explicit BitSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BleRel final : public Instruction2Byte {
 public:
  explicit BleRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BlsRel final : public Instruction2Byte {
 public:
  explicit BlsRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BltRel final : public Instruction2Byte {
 public:
  explicit BltRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BmcRel final : public Instruction2Byte {
 public:
  explicit BmcRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BmiRel final : public Instruction2Byte {
 public:
  explicit BmiRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BmsRel final : public Instruction2Byte {
 public:
  explicit BmsRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BneRel final : public Instruction2Byte {
 public:
  explicit BneRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BplRel final : public Instruction2Byte {
 public:
  explicit BplRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BraRel final : public Instruction2Byte {
 public:
  explicit BraRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB0 final : public Instruction3Byte {
 public:
  explicit BrclrDirB0();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB1 final : public Instruction3Byte {
 public:
  explicit BrclrDirB1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB2 final : public Instruction3Byte {
 public:
  explicit BrclrDirB2();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB3 final : public Instruction3Byte {
 public:
  explicit BrclrDirB3();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB4 final : public Instruction3Byte {
 public:
  explicit BrclrDirB4();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB5 final : public Instruction3Byte {
 public:
  explicit BrclrDirB5();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB6 final : public Instruction3Byte {
 public:
  explicit BrclrDirB6();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrclrDirB7 final : public Instruction3Byte {
 public:
  explicit BrclrDirB7();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrnRel final : public Instruction3Byte {
 public:
  explicit BrnRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB0 final : public Instruction3Byte {
 public:
  explicit BrsetDirB0();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB1 final : public Instruction3Byte {
 public:
  explicit BrsetDirB1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB2 final : public Instruction3Byte {
 public:
  explicit BrsetDirB2();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB3 final : public Instruction3Byte {
 public:
  explicit BrsetDirB3();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB4 final : public Instruction3Byte {
 public:
  explicit BrsetDirB4();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB5 final : public Instruction3Byte {
 public:
  explicit BrsetDirB5();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB6 final : public Instruction3Byte {
 public:
  explicit BrsetDirB6();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BrsetDirB7 final : public Instruction3Byte {
 public:
  explicit BrsetDirB7();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB0 final : public Instruction2Byte {
 public:
  explicit BsetDirB0();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB1 final : public Instruction2Byte {
 public:
  explicit BsetDirB1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB2 final : public Instruction2Byte {
 public:
  explicit BsetDirB2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB3 final : public Instruction2Byte {
 public:
  explicit BsetDirB3();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB4 final : public Instruction2Byte {
 public:
  explicit BsetDirB4();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB5 final : public Instruction2Byte {
 public:
  explicit BsetDirB5();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB6 final : public Instruction2Byte {
 public:
  explicit BsetDirB6();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsetDirB7 final : public Instruction2Byte {
 public:
  explicit BsetDirB7();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class BsrRel final : public Instruction2Byte {
 public:
  explicit BsrRel();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqDir final : public Instruction3Byte {
 public:
  explicit CbeqDir();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqaImm final : public Instruction3Byte {
 public:
  explicit CbeqaImm();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqxImm final : public Instruction3Byte {
 public:
  explicit CbeqxImm();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqIx1Postinc final : public Instruction3Byte {
 public:
  explicit CbeqIx1Postinc();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqIxPostinc final : public Instruction2Byte {
 public:
  explicit CbeqIxPostinc();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CbeqSp1 final : public Instruction4Byte {
 public:
  explicit CbeqSp1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClcInh final : public Instruction1Byte {
 public:
  explicit ClcInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CliInh final : public Instruction1Byte {
 public:
  explicit CliInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrDir final : public Instruction2Byte {
 public:
  explicit ClrDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClraInh final : public Instruction1Byte {
 public:
  explicit ClraInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrxInh final : public Instruction1Byte {
 public:
  explicit ClrxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrhInh final : public Instruction1Byte {
 public:
  explicit ClrhInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrIx1 final : public Instruction2Byte {
 public:
  explicit ClrIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrIx final : public Instruction1Byte {
 public:
  explicit ClrIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ClrSp1 final : public Instruction3Byte {
 public:
  explicit ClrSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpImm final : public Instruction2Byte {
 public:
  explicit CmpImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpDir final : public Instruction2Byte {
 public:
  explicit CmpDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpExt final : public Instruction3Byte {
 public:
  explicit CmpExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpIx2 final : public Instruction3Byte {
 public:
  explicit CmpIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpIx1 final : public Instruction2Byte {
 public:
  explicit CmpIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpIx final : public Instruction1Byte {
 public:
  explicit CmpIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpSp2 final : public Instruction4Byte {
 public:
  explicit CmpSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CmpSp1 final : public Instruction3Byte {
 public:
  explicit CmpSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComDir final : public Instruction2Byte {
 public:
  explicit ComDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComaInh final : public Instruction1Byte {
 public:
  explicit ComaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComxInh final : public Instruction1Byte {
 public:
  explicit ComxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComIx1 final : public Instruction2Byte {
 public:
  explicit ComIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComIx final : public Instruction1Byte {
 public:
  explicit ComIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class ComSp1 final : public Instruction3Byte {
 public:
  explicit ComSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CphxExt final : public Instruction3Byte {
 public:
  explicit CphxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CphxImm final : public Instruction3Byte {
 public:
  explicit CphxImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CphxDir final : public Instruction2Byte {
 public:
  explicit CphxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CphxSp1 final : public Instruction3Byte {
 public:
  explicit CphxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxImm final : public Instruction2Byte {
 public:
  explicit CpxImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxDir final : public Instruction2Byte {
 public:
  explicit CpxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxExt final : public Instruction3Byte {
 public:
  explicit CpxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxIx2 final : public Instruction3Byte {
 public:
  explicit CpxIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxIx1 final : public Instruction2Byte {
 public:
  explicit CpxIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxIx final : public Instruction1Byte {
 public:
  explicit CpxIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxSp2 final : public Instruction4Byte {
 public:
  explicit CpxSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class CpxSp1 final : public Instruction3Byte {
 public:
  explicit CpxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DaaInh final : public Instruction1Byte {
 public:
  explicit DaaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzDir final : public Instruction3Byte {
 public:
  explicit DbnzDir();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzaInh final : public Instruction2Byte {
 public:
  explicit DbnzaInh();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzxInh final : public Instruction2Byte {
 public:
  explicit DbnzxInh();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzIx1 final : public Instruction3Byte {
 public:
  explicit DbnzIx1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzIx final : public Instruction2Byte {
 public:
  explicit DbnzIx();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DbnzSp1 final : public Instruction4Byte {
 public:
  explicit DbnzSp1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecDir final : public Instruction2Byte {
 public:
  explicit DecDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecaInh final : public Instruction1Byte {
 public:
  explicit DecaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecxInh final : public Instruction1Byte {
 public:
  explicit DecxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecIx1 final : public Instruction2Byte {
 public:
  explicit DecIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecIx final : public Instruction1Byte {
 public:
  explicit DecIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DecSp1 final : public Instruction3Byte {
 public:
  explicit DecSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class DivInh final : public Instruction1Byte {
 public:
  explicit DivInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorImm final : public Instruction2Byte {
 public:
  explicit EorImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorDir final : public Instruction2Byte {
 public:
  explicit EorDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorExt final : public Instruction3Byte {
 public:
  explicit EorExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorIx2 final : public Instruction3Byte {
 public:
  explicit EorIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorIx1 final : public Instruction2Byte {
 public:
  explicit EorIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorIx final : public Instruction1Byte {
 public:
  explicit EorIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorSp2 final : public Instruction4Byte {
 public:
  explicit EorSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class EorSp1 final : public Instruction3Byte {
 public:
  explicit EorSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncDir final : public Instruction2Byte {
 public:
  explicit IncDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncaInh final : public Instruction1Byte {
 public:
  explicit IncaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncxInh final : public Instruction1Byte {
 public:
  explicit IncxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncIx1 final : public Instruction2Byte {
 public:
  explicit IncIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncIx final : public Instruction1Byte {
 public:
  explicit IncIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class IncSp1 final : public Instruction3Byte {
 public:
  explicit IncSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JmpDir final : public Instruction2Byte {
 public:
  explicit JmpDir();

  static uint8_t GetTarget(const uint8_t* opcode);

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JmpExt final : public Instruction3Byte {
 public:
  explicit JmpExt();

  static uint16_t GetTarget(const uint8_t* opcode);

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JmpIx2 final : public Instruction3Byte {
 public:
  explicit JmpIx2();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JmpIx1 final : public Instruction2Byte {
 public:
  explicit JmpIx1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JmpIx final : public Instruction1Byte {
 public:
  explicit JmpIx();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JsrDir final : public Instruction2Byte {
 public:
  explicit JsrDir();

  static uint8_t GetTarget(const uint8_t* opcode);

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JsrExt final : public Instruction3Byte {
 public:
  explicit JsrExt();

  static uint16_t GetTarget(const uint8_t* opcode);

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JsrIx2 final : public Instruction3Byte {
 public:
  explicit JsrIx2();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JsrIx1 final : public Instruction2Byte {
 public:
  explicit JsrIx1();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class JsrIx final : public Instruction1Byte {
 public:
  explicit JsrIx();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaImm final : public Instruction2Byte {
 public:
  explicit LdaImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaDir final : public Instruction2Byte {
 public:
  explicit LdaDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaExt final : public Instruction3Byte {
 public:
  explicit LdaExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaIx2 final : public Instruction3Byte {
 public:
  explicit LdaIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaIx1 final : public Instruction2Byte {
 public:
  explicit LdaIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaIx final : public Instruction1Byte {
 public:
  explicit LdaIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaSp2 final : public Instruction4Byte {
 public:
  explicit LdaSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdaSp1 final : public Instruction3Byte {
 public:
  explicit LdaSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxImm final : public Instruction3Byte {
 public:
  explicit LdhxImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxDir final : public Instruction2Byte {
 public:
  explicit LdhxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxExt final : public Instruction3Byte {
 public:
  explicit LdhxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxIx final : public Instruction2Byte {
 public:
  explicit LdhxIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxIx2 final : public Instruction4Byte {
 public:
  explicit LdhxIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxIx1 final : public Instruction3Byte {
 public:
  explicit LdhxIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdhxSp1 final : public Instruction3Byte {
 public:
  explicit LdhxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxImm final : public Instruction2Byte {
 public:
  explicit LdxImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxDir final : public Instruction2Byte {
 public:
  explicit LdxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxExt final : public Instruction3Byte {
 public:
  explicit LdxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxIx2 final : public Instruction3Byte {
 public:
  explicit LdxIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxIx1 final : public Instruction2Byte {
 public:
  explicit LdxIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxIx final : public Instruction1Byte {
 public:
  explicit LdxIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxSp2 final : public Instruction4Byte {
 public:
  explicit LdxSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LdxSp1 final : public Instruction3Byte {
 public:
  explicit LdxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslDir final : public Instruction2Byte {
 public:
  explicit LslDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslaInh final : public Instruction1Byte {
 public:
  explicit LslaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslxInh final : public Instruction1Byte {
 public:
  explicit LslxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslIx1 final : public Instruction2Byte {
 public:
  explicit LslIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslIx final : public Instruction1Byte {
 public:
  explicit LslIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LslSp1 final : public Instruction3Byte {
 public:
  explicit LslSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsrDir final : public Instruction2Byte {
 public:
  explicit LsrDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsraInh final : public Instruction1Byte {
 public:
  explicit LsraInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsrxInh final : public Instruction1Byte {
 public:
  explicit LsrxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsrIx1 final : public Instruction2Byte {
 public:
  explicit LsrIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsrIx final : public Instruction1Byte {
 public:
  explicit LsrIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class LsrSp1 final : public Instruction3Byte {
 public:
  explicit LsrSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class MovDirDir final : public Instruction3Byte {
 public:
  explicit MovDirDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class MovDirIxPostinc final : public Instruction2Byte {
 public:
  explicit MovDirIxPostinc();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class MovImmDir final : public Instruction3Byte {
 public:
  explicit MovImmDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class MovIxPostincDir final : public Instruction2Byte {
 public:
  explicit MovIxPostincDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class MulInh final : public Instruction1Byte {
 public:
  explicit MulInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegDir final : public Instruction2Byte {
 public:
  explicit NegDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegaInh final : public Instruction1Byte {
 public:
  explicit NegaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegxInh final : public Instruction1Byte {
 public:
  explicit NegxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegIx1 final : public Instruction2Byte {
 public:
  explicit NegIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegIx final : public Instruction1Byte {
 public:
  explicit NegIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NegSp1 final : public Instruction3Byte {
 public:
  explicit NegSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NopInh final : public Instruction1Byte {
 public:
  explicit NopInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class NsaInh final : public Instruction1Byte {
 public:
  explicit NsaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraImm final : public Instruction2Byte {
 public:
  explicit OraImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraDir final : public Instruction2Byte {
 public:
  explicit OraDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraExt final : public Instruction3Byte {
 public:
  explicit OraExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraIx2 final : public Instruction3Byte {
 public:
  explicit OraIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraIx1 final : public Instruction2Byte {
 public:
  explicit OraIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraIx final : public Instruction1Byte {
 public:
  explicit OraIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraSp2 final : public Instruction4Byte {
 public:
  explicit OraSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class OraSp1 final : public Instruction3Byte {
 public:
  explicit OraSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PshaInh final : public Instruction1Byte {
 public:
  explicit PshaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PshhInh final : public Instruction1Byte {
 public:
  explicit PshhInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PshxInh final : public Instruction1Byte {
 public:
  explicit PshxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PulaInh final : public Instruction1Byte {
 public:
  explicit PulaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PulhInh final : public Instruction1Byte {
 public:
  explicit PulhInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class PulxInh final : public Instruction1Byte {
 public:
  explicit PulxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolDir final : public Instruction2Byte {
 public:
  explicit RolDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolaInh final : public Instruction1Byte {
 public:
  explicit RolaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolxInh final : public Instruction1Byte {
 public:
  explicit RolxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolIx1 final : public Instruction2Byte {
 public:
  explicit RolIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolIx final : public Instruction1Byte {
 public:
  explicit RolIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RolSp1 final : public Instruction3Byte {
 public:
  explicit RolSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RorDir final : public Instruction2Byte {
 public:
  explicit RorDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RoraInh final : public Instruction1Byte {
 public:
  explicit RoraInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RorxInh final : public Instruction1Byte {
 public:
  explicit RorxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RorIx1 final : public Instruction2Byte {
 public:
  explicit RorIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RorIx final : public Instruction1Byte {
 public:
  explicit RorIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RorSp1 final : public Instruction3Byte {
 public:
  explicit RorSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RspInh final : public Instruction1Byte {
 public:
  explicit RspInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RtiInh final : public Instruction1Byte {
 public:
  explicit RtiInh();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class RtsInh final : public Instruction1Byte {
 public:
  explicit RtsInh();

  bool Info(const uint8_t* opcode, uint64_t addr,
            BN::InstructionInfo& result) override;

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcImm final : public Instruction2Byte {
 public:
  explicit SbcImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcDir final : public Instruction2Byte {
 public:
  explicit SbcDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcExt final : public Instruction3Byte {
 public:
  explicit SbcExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcIx2 final : public Instruction3Byte {
 public:
  explicit SbcIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcIx1 final : public Instruction2Byte {
 public:
  explicit SbcIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcIx final : public Instruction1Byte {
 public:
  explicit SbcIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcSp2 final : public Instruction4Byte {
 public:
  explicit SbcSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SbcSp1 final : public Instruction3Byte {
 public:
  explicit SbcSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SecInh final : public Instruction1Byte {
 public:
  explicit SecInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SeiInh final : public Instruction1Byte {
 public:
  explicit SeiInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaDir final : public Instruction2Byte {
 public:
  explicit StaDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaExt final : public Instruction3Byte {
 public:
  explicit StaExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaIx2 final : public Instruction3Byte {
 public:
  explicit StaIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaIx1 final : public Instruction2Byte {
 public:
  explicit StaIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaIx final : public Instruction1Byte {
 public:
  explicit StaIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaSp2 final : public Instruction4Byte {
 public:
  explicit StaSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StaSp1 final : public Instruction3Byte {
 public:
  explicit StaSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SthxDir final : public Instruction2Byte {
 public:
  explicit SthxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SthxExt final : public Instruction3Byte {
 public:
  explicit SthxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SthxSp1 final : public Instruction3Byte {
 public:
  explicit SthxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StopInh final : public Instruction1Byte {
 public:
  explicit StopInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxDir final : public Instruction2Byte {
 public:
  explicit StxDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxExt final : public Instruction3Byte {
 public:
  explicit StxExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxIx2 final : public Instruction3Byte {
 public:
  explicit StxIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxIx1 final : public Instruction2Byte {
 public:
  explicit StxIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxIx final : public Instruction1Byte {
 public:
  explicit StxIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxSp2 final : public Instruction4Byte {
 public:
  explicit StxSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class StxSp1 final : public Instruction3Byte {
 public:
  explicit StxSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubImm final : public Instruction2Byte {
 public:
  explicit SubImm();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubDir final : public Instruction2Byte {
 public:
  explicit SubDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubExt final : public Instruction3Byte {
 public:
  explicit SubExt();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubIx2 final : public Instruction3Byte {
 public:
  explicit SubIx2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubIx1 final : public Instruction2Byte {
 public:
  explicit SubIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubIx final : public Instruction1Byte {
 public:
  explicit SubIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubSp2 final : public Instruction4Byte {
 public:
  explicit SubSp2();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SubSp1 final : public Instruction3Byte {
 public:
  explicit SubSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class SwiInh final : public Instruction1Byte {
 public:
  explicit SwiInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TapInh final : public Instruction1Byte {
 public:
  explicit TapInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TaxInh final : public Instruction1Byte {
 public:
  explicit TaxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TpaInh final : public Instruction1Byte {
 public:
  explicit TpaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstDir final : public Instruction2Byte {
 public:
  explicit TstDir();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstaInh final : public Instruction1Byte {
 public:
  explicit TstaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstxInh final : public Instruction1Byte {
 public:
  explicit TstxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstIx1 final : public Instruction2Byte {
 public:
  explicit TstIx1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstIx final : public Instruction1Byte {
 public:
  explicit TstIx();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TstSp1 final : public Instruction3Byte {
 public:
  explicit TstSp1();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TsxInh final : public Instruction1Byte {
 public:
  explicit TsxInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TxaInh final : public Instruction1Byte {
 public:
  explicit TxaInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class TxsInh final : public Instruction1Byte {
 public:
  explicit TxsInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

class WaitInh final : public Instruction1Byte {
 public:
  explicit WaitInh();

  bool Text(const uint8_t* opcode, uint64_t addr, size_t& len,
            std::vector<BN::InstructionTextToken>& result) override;

  bool Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
            BN::LowLevelILFunction& il, BN::Architecture* arch) override;
};

}  // namespace HCS08

#endif  // HCS08_INSTRUCTIONS_H
