// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "info.h"

#include <binaryninjaapi.h>

#include "instructions.h"
#include "opcodes.h"
#include "sizes.h"

namespace HCS08 {
/* Default Instruction class info methods */
bool Instruction1Byte::Info(const uint8_t* opcode, uint64_t addr,
                            BN::InstructionInfo& result) {
  result.length = length;
  return true;
}

bool Instruction2Byte::Info(const uint8_t* opcode, uint64_t addr,
                            BN::InstructionInfo& result) {
  result.length = length;
  return true;
}

bool Instruction3Byte::Info(const uint8_t* opcode, uint64_t addr,
                            BN::InstructionInfo& result) {
  result.length = length;
  return true;
}

bool Instruction4Byte::Info(const uint8_t* opcode, uint64_t addr,
                            BN::InstructionInfo& result) {
  result.length = length;
  return true;
}

/* Helper Functions */
bool InfoBranch(const uint8_t* opcode, const uint64_t addr,
                BN::InstructionInfo& result) {
  result.length = Sizes::INSTRUCTION_2_BYTES;

  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget =
      falseBranchTarget + static_cast<int8_t>(opcode[1]);

  // Add branch behavior information
  switch (opcode[0]) {
    case Opcodes::BRA_REL:  // Branch always
      result.AddBranch(UnconditionalBranch, trueBranchTarget);
      return true;

    case Opcodes::BRN_REL:  // Branch never; Basically a 2-byte NOP
      // Do nothing
      return true;

    case Opcodes::BEQ_REL:      // Branch if equal
    case Opcodes::BNE_REL:      // Branch if not equal
    case Opcodes::BCC_BHS_REL:  // Branch if carry bit clear OR branch if higher
                                // or same
    case Opcodes::BCS_BLO_REL:  // Branch if carry bit set OR branch if lower
    case Opcodes::BPL_REL:      // Branch if plus
    case Opcodes::BMI_REL:      // Branch if minus
    case Opcodes::BIL_REL:      // Branch if IRQ pin low
    case Opcodes::BIH_REL:      // Branch if IRQ pin high
    case Opcodes::BMC_REL:      // Branch if interrupt mask clear
    case Opcodes::BMS_REL:      // Branch if interrupt mask set
    case Opcodes::BHCC_REL:     // Branch if half carry bit clear
    case Opcodes::BHCS_REL:     // Branch if half carry bit set
    case Opcodes::BLT_REL:      // Branch if less than (signed operands)
    case Opcodes::BLE_REL:  // Branch if less than or equal to (signed operands)
    case Opcodes::BGE_REL:  // Branch if greater than or equal to (signed
                            // operands)
    case Opcodes::BGT_REL:  // Branch if greater than (signed operands)
    case Opcodes::BLS_REL:  // Branch if lower or same
    case Opcodes::BHI_REL:  // Branch if higher
      result.AddBranch(TrueBranch, trueBranchTarget);
      result.AddBranch(FalseBranch, falseBranchTarget);
      return true;
    default:
      return false;
  }
}

bool InfoBrsetBrclr(const int8_t relOffset, const uint64_t addr,
                    BN::InstructionInfo& result) {
  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget = falseBranchTarget + relOffset;

  // Add branch info - note, branch depends on memory addr and may not be able
  // to be determined
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

bool InfoCbeqDirImm(const int8_t relOffset, const uint64_t addr,
                    BN::InstructionInfo& result) {
  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget = falseBranchTarget + relOffset;

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

bool InfoCbeqIxPlus(const int8_t relOffset, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = Sizes::INSTRUCTION_2_BYTES;

  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget = falseBranchTarget + relOffset;

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

bool InfoDbnzDirIx1(const int8_t relOffset, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = Sizes::INSTRUCTION_3_BYTES;

  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget = falseBranchTarget + relOffset;

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

bool InfoDbnzInhIx(const int8_t relOffset, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = Sizes::INSTRUCTION_2_BYTES;

  // Calculate branch targets
  const uint16_t falseBranchTarget = addr + result.length;
  const uint16_t trueBranchTarget = falseBranchTarget + relOffset;

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

/* Instruction info implementation overrides */
bool BccBhsRel::Info(const uint8_t* opcode, const uint64_t addr,
                     BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BcsBloRel::Info(const uint8_t* opcode, const uint64_t addr,
                     BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BeqRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BgeRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BgtRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BhccRel::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BhcsRel::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BhiRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BihRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BilRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BleRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BlsRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BltRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BmcRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BmiRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BmsRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BneRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BplRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BraRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BrclrDirB0::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB1::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB2::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB3::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB4::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB5::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB6::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrclrDirB7::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrnRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoBranch(opcode, addr, result);
}

bool BrsetDirB0::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB1::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB2::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB3::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB4::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB5::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB6::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BrsetDirB7::Info(const uint8_t* opcode, const uint64_t addr,
                      BN::InstructionInfo& result) {
  result.length = length;
  return InfoBrsetBrclr(opcode[2], addr, result);
}

bool BsrRel::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  uint16_t callTarget = addr + result.length + opcode[1];
  result.AddBranch(CallDestination, callTarget);
  return true;
}

bool CbeqDir::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;
  return InfoCbeqDirImm(opcode[2], addr, result);
}

bool CbeqaImm::Info(const uint8_t* opcode, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = length;
  return InfoCbeqDirImm(opcode[2], addr, result);
}

bool CbeqIxPostinc::Info(const uint8_t* opcode, const uint64_t addr,
                         BN::InstructionInfo& result) {
  result.length = length;
  return InfoCbeqIxPlus(opcode[1], addr, result);
}

bool CbeqIx1Postinc::Info(const uint8_t* opcode, const uint64_t addr,
                          BN::InstructionInfo& result) {
  result.length = length;
  return InfoCbeqIxPlus(opcode[1], addr, result);
}

bool CbeqxImm::Info(const uint8_t* opcode, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = length;
  return InfoCbeqDirImm(opcode[2], addr, result);
}

bool CbeqSp1::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;

  // Calculate branch targets
  uint16_t falseBranchTarget = addr + result.length;
  uint16_t trueBranchTarget = falseBranchTarget + opcode[3];

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

bool DbnzDir::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;
  return InfoDbnzDirIx1(opcode[2], addr, result);
}

bool DbnzaInh::Info(const uint8_t* opcode, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = length;
  return InfoDbnzInhIx(opcode[1], addr, result);
}

bool DbnzxInh::Info(const uint8_t* opcode, const uint64_t addr,
                    BN::InstructionInfo& result) {
  result.length = length;
  return InfoDbnzInhIx(opcode[1], addr, result);
}

bool DbnzIx::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  return InfoDbnzInhIx(opcode[1], addr, result);
}

bool DbnzIx1::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = length;
  return InfoDbnzDirIx1(opcode[2], addr, result);
}

bool DbnzSp1::Info(const uint8_t* opcode, const uint64_t addr,
                   BN::InstructionInfo& result) {
  result.length = Sizes::INSTRUCTION_4_BYTES;

  // Calculate branch targets
  uint16_t falseBranchTarget = addr + result.length;
  uint16_t trueBranchTarget = falseBranchTarget + opcode[3];

  // Add branch info
  result.AddBranch(TrueBranch, trueBranchTarget);
  result.AddBranch(FalseBranch, falseBranchTarget);

  return true;
}

uint8_t JmpDir::GetTarget(const uint8_t* opcode) { return opcode[1]; }

bool JmpDir::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch, GetTarget(opcode));
  return true;
}

uint16_t JmpExt::GetTarget(const uint8_t* opcode) {
  return opcode[1] << 8 | opcode[2];
}

bool JmpExt::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch, GetTarget(opcode));
  return true;
}

bool JmpIx::Info(const uint8_t* opcode, const uint64_t addr,
                 BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch);
  return true;
}

bool JmpIx1::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch);
  return true;
}

bool JmpIx2::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(UnconditionalBranch);
  return true;
}

uint8_t JsrDir::GetTarget(const uint8_t* opcode) { return opcode[1]; }

bool JsrDir::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(CallDestination, GetTarget(opcode));
  return true;
}

uint16_t JsrExt::GetTarget(const uint8_t* opcode) {
  return opcode[1] << 8 | opcode[2];
}

bool JsrExt::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(CallDestination, GetTarget(opcode));
  return true;
}

bool JsrIx::Info(const uint8_t* opcode, const uint64_t addr,
                 BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(IndirectBranch);
  return true;
}

bool JsrIx1::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(IndirectBranch);
  return true;
}

bool JsrIx2::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(IndirectBranch);
  return true;
}

bool RtsInh::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(FunctionReturn);
  return true;
}

bool RtiInh::Info(const uint8_t* opcode, const uint64_t addr,
                  BN::InstructionInfo& result) {
  result.length = length;
  result.AddBranch(FunctionReturn);
  return true;
}

}  // namespace HCS08
