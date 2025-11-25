// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "lift.h"

#include <binaryninjaapi.h>

#include <cstdint>

#include "flags.h"
#include "instructions.h"
#include "opcodes.h"
#include "registers.h"
#include "sizes.h"

namespace BN = BinaryNinja;

#define IX_STORE(x) \
  il.Store(Sizes::BYTE, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), x)
#define IX1_STORE(x)                                                         \
  il.Store(                                                                  \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::BYTE, oprx8)),                                  \
      x)
#define IX2_STORE(x)                                                         \
  il.Store(                                                                  \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::WORD, oprx16)),                                 \
      x)
#define SP_STORE(x) \
  il.Store(Sizes::BYTE, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), x)
#define SP1_STORE(x)                                                         \
  il.Store(                                                                  \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx8)),                                  \
      x)
#define SP2_STORE(x)                                                         \
  il.Store(                                                                  \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx16)),                                 \
      x)

#define IX_STORE_WORD(x) \
  il.Store(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), x)
#define IX1_STORE_WORD(x)                                                    \
  il.Store(                                                                  \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::BYTE, oprx8)),                                  \
      x)
#define IX2_STORE_WORD(x)                                                    \
  il.Store(                                                                  \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::WORD, oprx16)),                                 \
      x)
#define SP_STORE_WORD(x) \
  il.Store(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), x)
#define SP1_STORE_WORD(x)                                                    \
  il.Store(                                                                  \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx8)),                                  \
      x)
#define SP2_STORE_WORD(x)                                                    \
  il.Store(                                                                  \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx16)),                                 \
      x)

#define IX_LOAD \
  il.Load(Sizes::BYTE, il.Register(Sizes::WORD, Registers::HCS08_REG_HX))
#define IX1_LOAD                                                             \
  il.Load(                                                                   \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::BYTE, oprx8)))
#define IX2_LOAD                                                             \
  il.Load(                                                                   \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::WORD, oprx16)))
#define SP_LOAD \
  il.Load(Sizes::BYTE, il.Register(Sizes::WORD, Registers::HCS08_REG_SP))
#define SP1_LOAD                                                             \
  il.Load(                                                                   \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx8)))
#define SP2_LOAD                                                             \
  il.Load(                                                                   \
      Sizes::BYTE,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx16)))

#define IX_LOAD_WORD \
  il.Load(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX))
#define IX1_LOAD_WORD                                                        \
  il.Load(                                                                   \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::BYTE, oprx8)))
#define IX2_LOAD_WORD                                                        \
  il.Load(                                                                   \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX), \
             il.Const(Sizes::WORD, oprx16)))
#define SP1_LOAD_WORD                                                        \
  il.Load(                                                                   \
      Sizes::WORD,                                                           \
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP), \
             il.Const(Sizes::BYTE, oprx8)))
#define UNIMPLEMENTED                    \
  len = length;                          \
  il.AddInstruction(il.Unimplemented()); \
  return true;

namespace HCS08 {
/* Instruction Implementations */
bool AdcDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                  Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
                  Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  il.Const(Sizes::BYTE, imm8), Flags::FLAG_C,
                  Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  IX_LOAD, Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  IX1_LOAD, Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  IX2_LOAD, Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  SP1_LOAD, Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AdcSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.AddCarry(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                  SP2_LOAD, Flags::FLAG_C, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, imm8), Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX_LOAD, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX1_LOAD, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX2_LOAD, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP1_LOAD, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AddSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP2_LOAD, Flags::FLAGS_C_Z_N_H_V)));
  return true;
}

bool AisImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t opr8i = opcode[1];
  // Sign-extend relative offset to 16-bit int
  int16_t signExtImm;
  if (opr8i >> 7) {  // Negative
    signExtImm = 0xFF00 | opr8i;
  } else {  // Positive
    signExtImm = opr8i;
  }
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, signExtImm))));
  return true;
}

bool AixImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  // Sign-extend relative offset to 16-bit int
  int16_t signExtImm;
  if (imm8 >> 7) {  // Negative
    signExtImm = 0xFF00 | imm8;
  } else {  // Positive
    signExtImm = imm8;
  }
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, signExtImm))));
  return true;
}

bool AndDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, imm8), Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool AndSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool AsraInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.ArithShiftRight(Sizes::BYTE,
                         il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                         il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool AsrDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.ArithShiftRight(
                   Sizes::BYTE,
                   il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                   il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool AsrIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.ArithShiftRight(
      Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool AsrIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.ArithShiftRight(
      Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool AsrSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.ArithShiftRight(
      Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool AsrxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.ArithShiftRight(Sizes::BYTE,
                         il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                         il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool BccBhsRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BclrDirB0::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB3::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB4::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB5::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB6::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BclrDirB7::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BcsBloRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BeqRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BgeRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BgndInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  UNIMPLEMENTED
}

bool BgtRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BhccRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BhcsRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BhiRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BihRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BilRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BitDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_Z_N_V));
  return true;
}

bool BitExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_Z_N_V));
  return true;
}

bool BitImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, imm8), Flags::FLAGS_Z_N_V));
  return true;
}

bool BitIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX_LOAD, Flags::FLAGS_Z_N_V));
  return true;
}

bool BitIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX1_LOAD, Flags::FLAGS_Z_N_V));
  return true;
}

bool BitIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX2_LOAD, Flags::FLAGS_Z_N_V));
  return true;
}

bool BitSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           SP1_LOAD, Flags::FLAGS_Z_N_V));
  return true;
}

bool BitSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.And(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           SP2_LOAD, Flags::FLAGS_Z_N_V));
  return true;
}

bool BleRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BltRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
}

bool BlsRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BmcRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BmiRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BmsRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BneRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BplRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BraRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BrclrDirB0::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB3::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB4::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB5::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB6::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrclrDirB7::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrnRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBranchRel(opcode, addr, len, il, arch);
  ;
}

bool BrsetDirB0::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB3::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB4::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB5::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB6::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BrsetDirB7::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                      BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBrsetBrclrDir(opcode, addr, len, il, arch);
}

bool BsetDirB0::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB3::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB4::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB5::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB6::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsetDirB7::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  return LiftBsetBclrDir(opcode, len, il);
}

bool BsrRel::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t relOffset = opcode[1];  // Offset used to calculate branch target
  uint16_t destAddr = addr + len;
  // Save return address to stack
  il.AddInstruction(il.Push(Sizes::WORD, il.Const(Sizes::WORD, destAddr)));
  il.AddInstruction(il.Call(il.ConstPointer(
      Sizes::WORD, destAddr + static_cast<int16_t>(relOffset))));
  return true;
}

bool CbeqaImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                    BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t opr8i = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, opr8i), Flags::FLAGS_C_Z_N_V));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool CbeqDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t opr8a = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_V));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool CbeqIxPostinc::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                         BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  int8_t relOffset =
      opcode[1];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             IX_LOAD, Flags::FLAGS_C_Z_N_V));
  il.AddInstruction(  // Increment index register
      il.SetRegister(
          Sizes::WORD, Registers::HCS08_REG_HX,
          il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                 il.Const(Sizes::WORD, 1))));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool CbeqIx1Postinc::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                          BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t oprx8 = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             IX1_LOAD, Flags::FLAGS_C_Z_N_V));
  il.AddInstruction(  // Increment index register
      il.SetRegister(
          Sizes::WORD, Registers::HCS08_REG_HX,
          il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                 il.Const(Sizes::WORD, 1))));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool CbeqxImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                    BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t opr8i = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Const(Sizes::BYTE, opr8i), Flags::FLAGS_C_Z_N_V));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool CbeqSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t oprx8 = opcode[2];
  int8_t relOffset =
      opcode[3];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Compare operand against the accumulator
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             SP1_LOAD, Flags::FLAGS_C_Z_N_V));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_E), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool ClcInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetFlag(Flags::FLAG_C, il.Const(Sizes::BYTE, 0)));
  return true;
}

bool CliInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetFlag(Flags::FLAG_I, il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClraInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(Sizes::BYTE,             // Size of register
                                   Registers::HCS08_REG_A,  // Register to set
                                   il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
                             il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrhInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(Sizes::BYTE,             // Size of register
                                   Registers::HCS08_REG_A,  // Register to set
                                   il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Const(Sizes::BYTE, 0)));
  return true;
}

bool ClrxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.And(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, 0xFF00))));  // Clears X,
  return true;
}

bool CmpDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, imm8), Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX1_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           IX2_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           SP1_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CmpSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           SP2_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool ComaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE,             // Size of register
                     Registers::HCS08_REG_A,  // Register to set
                     il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                            il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                            Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool ComDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                      Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool ComIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                                    IX_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool ComIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                                     IX1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool ComSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                                     SP1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool ComxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X,
                     il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                            il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                            Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool CphxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(
      il.Sub(Sizes::WORD, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CphxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.Sub(Sizes::WORD, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CphxImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t opr16i = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.Sub(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, opr16i), Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CphxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(
      Sizes::WORD, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
      il.Load(
          Sizes::WORD,
          il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
                 il.Const(Sizes::BYTE,
                          oprx8))),  // NOTE: couldn't just use
                                     // SP1_LOAD macro since that
                                     // operates on a Sizes::BYTE rather
                                     // than a Sizes::WORD. I could have
                                     // made a new macro, but didn't
      Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, imm8), Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           IX_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           IX1_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           IX2_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           SP1_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool CpxSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           SP2_LOAD, Flags::FLAGS_C_Z_N_V));
  return true;
}

bool DaaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  UNIMPLEMENTED
}

bool DbnzaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                    BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  int8_t relOffset =
      opcode[1];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement A
      il.SetRegister(
          Sizes::BYTE, Registers::HCS08_REG_A,
          il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z)));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DbnzDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t opr8a = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement value stored at specified memory address
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Sub(Sizes::BYTE,
                      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                      il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z)));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DbnzIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  int8_t relOffset =
      opcode[1];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement value stored in indexed memory
      IX_STORE(il.Sub(Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1),
                      Flags::FLAGS_Z)));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DbnzIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t oprx8 = opcode[1];
  int8_t relOffset =
      opcode[2];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement value stored in indexed memory
      IX1_STORE(il.Sub(Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1),
                       Flags::FLAGS_Z)));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DbnzSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  uint8_t oprx8 = opcode[2];
  int8_t relOffset =
      opcode[3];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement value stored in stack memory
      SP1_STORE(il.Sub(Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1),
                       Flags::FLAGS_Z)));
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DbnzxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                    BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Parse operands
  int8_t relOffset =
      opcode[1];      // rel operand is used to calculate branch target
  il.AddInstruction(  // Decrement X
      il.SetRegister(
          Sizes::BYTE, Registers::HCS08_REG_HX,
          il.And(Sizes::WORD,
                 il.And(Sizes::BYTE,
                        il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                        il.Const(Sizes::WORD, 0xFF00)),  // H; avoid
                                                         // changing it
                 il.Sub(Sizes::BYTE,
                        il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                        il.Const(Sizes::BYTE, 1),
                        Flags::FLAGS_Z))));  // X - 1; set Z flag
  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;
  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);
  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }
  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }
  // Branch if operand and accumulator register are not equal
  il.AddInstruction(il.If(il.FlagCondition(LLFC_NE), true_label, false_label));
  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }
  if (!found_false_label) {
    il.MarkLabel(false_label);
  }
  return true;
}

bool DecaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DecDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Sub(Sizes::BYTE,
                      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                      il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DecIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.Sub(
      Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DecIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Sub(
      Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DecSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Sub(
      Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DecxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool DivInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Shenanigans, bc I decided to make H:X one 16-bit register so indexing would
  // be easy. So now this is hard.
  il.AddInstruction(  // Get quotient and store in register A
      il.SetRegister(
          Sizes::BYTE, Registers::HCS08_REG_A,
          il.DivDoublePrecUnsigned(
              Sizes::WORD,
              il.And(Sizes::WORD,  // H:A - dividend
                     il.And(Sizes::WORD,
                            il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                            il.Const(Sizes::WORD, 0xFF00)),
                     il.Register(Sizes::BYTE, Registers::HCS08_REG_A)),
              il.Register(Sizes::BYTE, Registers::HCS08_REG_X),  // X - divisor
              Flags::FLAGS_C_Z)));
  il.AddInstruction(  // Get remainder and store in register H
      il.SetRegister(
          Sizes::WORD, Registers::HCS08_REG_HX,
          il.And(
              Sizes::WORD,
              il.ShiftLeft(
                  Sizes::WORD,
                  il.ModDoublePrecUnsigned(
                      Sizes::WORD,
                      il.And(Sizes::WORD,  // H:A - dividend
                             il.And(Sizes::WORD,
                                    il.Register(Sizes::BYTE,
                                                Registers::HCS08_REG_X),
                                    il.Const(Sizes::WORD, 0xFF00)),
                             il.Register(Sizes::BYTE, Registers::HCS08_REG_A)),
                      il.Register(Sizes::BYTE,
                                  Registers::HCS08_REG_X)),  // X - divisor
                  il.Const(Sizes::BYTE, 8)),  // new value of H - the remainder
              il.Register(Sizes::BYTE,
                          Registers::HCS08_REG_X))));  // divisor X, which
                                                       // should be unchanged
  return true;
}

bool EorDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, imm8), Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool EorSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Xor(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Add(Sizes::BYTE,
                      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                      il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.Add(
      Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Add(
      Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Add(
      Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool IncxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Add(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
             il.Const(Sizes::BYTE, 1), Flags::FLAGS_Z_N_V)));
  return true;
}

bool JmpDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  il.AddInstruction(il.Jump(il.ConstPointer(Sizes::WORD, GetTarget(opcode))));
  return true;
}

bool JmpExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  il.AddInstruction(il.Jump(il.ConstPointer(Sizes::WORD, GetTarget(opcode))));
  return true;
}

bool JmpIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Jump(il.Register(Sizes::WORD, Registers::HCS08_REG_HX)));
  return true;
}

bool JmpIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.Jump(
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, oprx8))));
  return true;
}

bool JmpIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Jump(
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, oprx16))));
  return true;
}

bool JsrDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  il.AddInstruction(il.Call(il.ConstPointer(Sizes::WORD, GetTarget(opcode))));
  return true;
}

bool JsrExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  il.AddInstruction(il.Call(il.ConstPointer(Sizes::WORD, GetTarget(opcode))));
  return true;
}

bool JsrIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Call(il.Register(Sizes::WORD, Registers::HCS08_REG_HX)));
  return true;
}

bool JsrIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(
      il.Call(il.Add(Sizes::WORD, il.Const(Sizes::WORD, oprx8),
                     il.Register(Sizes::WORD, Registers::HCS08_REG_HX))));
  return true;
}

bool JsrIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.Call(il.Add(Sizes::WORD, il.Const(Sizes::WORD, oprx16),
                     il.Register(Sizes::WORD, Registers::HCS08_REG_HX))));
  return true;
}

bool LdaDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a))));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a))));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A,
                                   il.Const(Sizes::BYTE, imm8)));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, IX_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, IX1_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, IX2_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, SP1_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdaSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, SP2_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.Load(Sizes::WORD, il.ConstPointer(Sizes::WORD, opr8a))));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.Load(Sizes::WORD, il.ConstPointer(Sizes::WORD, opr16a))));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint16_t opr16i = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(Sizes::WORD, Registers::HCS08_REG_HX,
                                   il.Const(Sizes::WORD, opr16i)));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::WORD, Registers::HCS08_REG_HX, IX_LOAD_WORD));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(
      il.SetRegister(Sizes::WORD, Registers::HCS08_REG_HX, IX1_LOAD_WORD));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.SetRegister(Sizes::WORD, Registers::HCS08_REG_HX, IX2_LOAD_WORD));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdhxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(
      il.SetRegister(Sizes::WORD, Registers::HCS08_REG_HX, SP1_LOAD_WORD));
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a))));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a))));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X,
                                   il.Const(Sizes::BYTE, imm8)));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, IX_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, IX1_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, IX2_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, SP1_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LdxSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, SP2_LOAD));
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  return true;
}

bool LslaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.ShiftLeft(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                   il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LslDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(il.Store(
      Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
      il.ShiftLeft(Sizes::BYTE,
                   il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                   il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LslIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.ShiftLeft(
      Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LslIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.ShiftLeft(
      Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LslSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.ShiftLeft(
      Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LslxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.ShiftLeft(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                   il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsraInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.LogicalShiftRight(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsrDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.LogicalShiftRight(
                   Sizes::BYTE,
                   il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                   il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsrIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.LogicalShiftRight(
      Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsrIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.LogicalShiftRight(
      Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsrSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.LogicalShiftRight(
      Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool LsrxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.LogicalShiftRight(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V)));

  return true;
}

bool MovDirDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a_src = opcode[1];  // opr8a is low byte of address in direct
                                  // address space $0000-$00FF
  uint8_t opr8a_dst = opcode[2];
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a_dst),
               il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a_src))));
  // TODO: condition codes N, Z; clear V
  return true;
}

bool MovDirIxPostinc::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                           BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a_dst = opcode[1];  // opr8a is low byte of address in direct
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a_dst), IX_LOAD));
  il.AddInstruction(  // Increment index register
      il.SetRegister(
          Sizes::WORD, Registers::HCS08_REG_HX,
          il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                 il.Const(Sizes::WORD, 1))));
  return true;
}

bool MovImmDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                     BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8i_src = opcode[1];  // opr8a is low byte of address in direct
                                  // address space $0000-$00FF
  uint8_t opr8a_dst = opcode[2];
  il.AddInstruction(il.Store(Sizes::BYTE,
                             il.ConstPointer(Sizes::WORD, opr8a_dst),
                             il.Const(Sizes::BYTE, opr8i_src)));
  return true;
}

bool MovIxPostincDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                           BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a_src = opcode[1];  // opr8a is low byte of address in direct
  il.AddInstruction(
      IX_STORE(il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a_src))));
  il.AddInstruction(  // Increment index register
      il.SetRegister(
          Sizes::WORD, Registers::HCS08_REG_HX,
          il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                 il.Const(Sizes::WORD, 1))));
  return true;
}

bool MulInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // TODO? flags - FLAG_C and FLAG_H both get set to 0
  BN::ExprId result = il.MultDoublePrecUnsigned(
      Sizes::WORD, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
      il.Register(Sizes::BYTE, Registers::HCS08_REG_A));
  il.AddInstruction(  // Set X = high byte of result
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X,
                     il.And(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF),
                            il.LogicalShiftRight(Sizes::WORD, result,
                                                 il.Const(Sizes::BYTE, 8)))));
  il.AddInstruction(  // Set A = low byte of result
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A,
                     il.And(Sizes::BYTE, il.Const(Sizes::BYTE, 0xFF), result)));
  return true;
}

bool NegaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE,             // Size of register
                     Registers::HCS08_REG_A,  // Register to set
                     il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                            il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                            Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NegDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                      il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                      Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NegIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                                    IX_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NegIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                                     IX1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NegSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                                     SP1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NegxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X,
                     il.Sub(Sizes::BYTE, il.Const(Sizes::BYTE, 0),
                            il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                            Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool NopInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Nop());
  return true;
}

bool NsaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE,
                         il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                         il.Const(Sizes::BYTE, 4)),
            il.LogicalShiftRight(
                Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                il.Const(Sizes::BYTE, 4)))));
  return true;
}

bool OraDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
            Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
            Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            il.Const(Sizes::BYTE, imm8), Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            IX_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            IX1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            IX2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            SP1_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool OraSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Or(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
            SP2_LOAD, Flags::FLAGS_Z_N_V)));
  return true;
}

bool PshaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Store value of accumulator register A onto stack
  il.AddInstruction(SP_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));
  // Decrement stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Sub(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool PshhInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Register H (high byte of index register) requires a bit of extra logic, X
  // and A will be simpler
  il.AddInstruction(SP_STORE(il.LogicalShiftRight(
      Sizes::WORD,
      il.And(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, 0xFF00)),
      il.Const(Sizes::BYTE, 8))));
  // Decrement stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Sub(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool PshxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Store value of X, index register low, onto stack
  il.AddInstruction(SP_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));
  // Decrement stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Sub(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool PulaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Set register A (accumulator) to value popped from top of stack
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, SP_LOAD));
  // Increment stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool PulhInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Register H (high byte of index register) requires a bit of extra logic, X
  // and A will be simpler
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.Or(
          Sizes::WORD,
          il.ShiftLeft(Sizes::WORD, SP_LOAD, il.Const(Sizes::BYTE, 8)),
          il.And(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                 il.Const(Sizes::WORD, 0x00FF)))));  // Clearing the high byte
                                                     // of H:X so it can be
                                                     // set to the new value
                                                     // from the stack
  // Increment stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool PulxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Set X (index register low) to value popped from top of stack
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, SP_LOAD));
  // Increment stack pointer (because stack grows "down" to lower memory
  // addresses)
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool RolaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE,
                         il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                         il.Const(Sizes::BYTE, 7), Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RolDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(il.Store(
      Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
      il.Or(Sizes::BYTE,
            il.ShiftLeft(
                Sizes::BYTE,
                il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                il.Const(Sizes::BYTE, 7), Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RolIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 7),
                         Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RolIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 7),
                         Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RolSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 7),
                         Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RolxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Or(Sizes::BYTE,
            il.ShiftLeft(Sizes::BYTE,
                         il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                         il.Const(Sizes::BYTE, 7), Flags::FLAGS_C_Z_N_V),
            il.Flag(Flags::FLAG_C))));
  return true;
}

bool RoraInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE,             // Size of register
      Registers::HCS08_REG_A,  // Register to set
      il.Or(Sizes::BYTE,
            il.LogicalShiftRight(
                Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V),
            il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                         il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RorDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(il.Store(
      Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
      il.Or(Sizes::BYTE,
            il.LogicalShiftRight(
                Sizes::BYTE,
                il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V),
            il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                         il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RorIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(IX_STORE(
      il.Or(Sizes::BYTE,
            il.LogicalShiftRight(Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 1),
                                 Flags::FLAGS_C_Z_N_V),
            il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                         il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RorxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_X,
      il.Or(Sizes::BYTE,
            il.LogicalShiftRight(
                Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                il.Const(Sizes::BYTE, 1), Flags::FLAGS_C_Z_N_V),
            il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                         il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RorIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(IX1_STORE(il.Or(
      Sizes::BYTE,
      il.LogicalShiftRight(Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 1),
                           Flags::FLAGS_C_Z_N_V),
      il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                   il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RorSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(SP1_STORE(il.Or(
      Sizes::BYTE,
      il.LogicalShiftRight(Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 1),
                           Flags::FLAGS_C_Z_N_V),
      il.ShiftLeft(Sizes::BYTE, il.Flag(Flags::FLAG_C),
                   il.Const(Sizes::BYTE, 7)))));
  return true;
}

bool RspInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::WORD,
      Registers::HCS08_REG_SP,  // Instruction only sets LSB of SP, that's why
                                // it's being addressed as a Sizes::BYTE not a
                                // Sizes::WORD. MSB should be unaffected
      il.Or(Sizes::WORD, il.Register(Sizes::BYTE, Registers::HCS08_REG_SP),
            il.Const(Sizes::BYTE, 0xFF))));
  return true;
}

bool RtiInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Restore CCR, A, and X from stack
  il.AddInstruction(il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_CCR,
                                   il.Pop(Sizes::BYTE)));
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A, il.Pop(Sizes::BYTE)));
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X, il.Pop(Sizes::BYTE)));
  // PC is also restored from stack
  il.AddInstruction(il.Jump(il.Pop(Sizes::WORD)));
  return true;
}

bool RtsInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // PC is restored from stack
  il.AddInstruction(il.Return(il.Pop(Sizes::WORD)));
  return true;
}

bool SbcDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                   il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                   il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(
          Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
          il.Const(Sizes::BYTE, imm8), Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A), IX_LOAD,
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A), IX1_LOAD,
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A), IX2_LOAD,
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A), SP1_LOAD,
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SbcSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.SubBorrow(Sizes::BYTE,
                   il.Register(Sizes::BYTE, Registers::HCS08_REG_A), SP2_LOAD,
                   Flags::FLAG_C, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SecInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetFlag(Flags::FLAG_C, il.Const(Sizes::BYTE, 1)));

  return true;
}

bool SeiInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetFlag(Flags::FLAG_I, il.Const(Sizes::BYTE, 1)));
  return true;
}

bool StaDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a),
               il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX1_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX2_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      SP1_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool StaSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      SP2_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));  // Store A
  return true;
}

bool SthxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(il.Store(
      Sizes::WORD, il.ConstPointer(Sizes::WORD, opr8a),
      il.Register(Sizes::WORD, Registers::HCS08_REG_HX)));  // Store H:X
  return true;
}

bool SthxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(il.Store(
      Sizes::WORD, il.ConstPointer(Sizes::WORD, opr16a),
      il.Register(Sizes::WORD, Registers::HCS08_REG_HX)));  // Store H:X
  return true;
}

bool SthxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::WORD,
                           il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
                           il.Const(Sizes::WORD, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(SP1_STORE_WORD(
      il.Register(Sizes::WORD, Registers::HCS08_REG_HX)));  // Store H:X
  return true;
}

bool StopInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  UNIMPLEMENTED
}

bool StxDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
               il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      il.Store(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a),
               il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX1_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      IX2_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      SP1_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool StxSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));  // Set flags
  il.AddInstruction(
      SP2_STORE(il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));  // Store X
  return true;
}

bool SubDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr8a/opcode[1] is low byte of address in direct address space $0000-$00FF
  uint8_t opr8a = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
             Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubExt::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Opr16a is address anywhere in the 16-bit address space
  uint16_t opr16a = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr16a)),
             Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubImm::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int8_t imm8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, imm8), Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx8 is 8-bit offset from index register H:X
  int16_t oprx8 = opcode[1];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubIx2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Oprx16 is 16-bit offset from index register H:X
  int16_t oprx16 = opcode[1] << 8 | opcode[2];
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             IX2_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx16 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP1_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SubSp2::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx16 =
      opcode[2] << 8 |
      opcode[3];  // oprx16 operand is 16-bit offset from stack pointer
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_A,
      il.Sub(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             SP2_LOAD, Flags::FLAGS_C_Z_N_V)));
  return true;
}

bool SwiInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  UNIMPLEMENTED
}

bool TapInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, Registers::HCS08_REG_CCR,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, 0b01100000))));  // flag bits 5 and 6 are
                                                    // always 1 and not used
  // Set the flags! Because Binja isn't actually looking at the CCR, it's
  // tracking the flag states separately Logic: FLAG = (REG_A & (1 <<
  // BIT_N_OF_CCR_CORRESPONDING_TO_FLAG)) >> BIT_N_OF_CCR_CORRESPONDING_TO_FLAG
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_V,
      il.LogicalShiftRight(
          Sizes::BYTE,
          il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.ShiftLeft(Sizes::BYTE, il.Const(Sizes::BYTE, 1),
                              il.Const(Sizes::BYTE, Flags::FLAG_V))),
          il.Const(Sizes::BYTE, Flags::FLAG_V))));
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_H,
      il.LogicalShiftRight(
          Sizes::BYTE,
          il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.ShiftLeft(Sizes::BYTE, il.Const(Sizes::BYTE, 1),
                              il.Const(Sizes::BYTE, Flags::FLAG_H))),
          il.Const(Sizes::BYTE, Flags::FLAG_H))));
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_I,
      il.LogicalShiftRight(
          Sizes::BYTE,
          il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.ShiftLeft(Sizes::BYTE, il.Const(Sizes::BYTE, 1),
                              il.Const(Sizes::BYTE, Flags::FLAG_I))),
          il.Const(Sizes::BYTE, Flags::FLAG_I))));
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_N,
      il.LogicalShiftRight(
          Sizes::BYTE,
          il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.ShiftLeft(Sizes::BYTE, il.Const(Sizes::BYTE, 1),
                              il.Const(Sizes::BYTE, Flags::FLAG_N))),
          il.Const(Sizes::BYTE, Flags::FLAG_N))));
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_Z,
      il.LogicalShiftRight(
          Sizes::BYTE,
          il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                 il.ShiftLeft(Sizes::BYTE, il.Const(Sizes::BYTE, 1),
                              il.Const(Sizes::BYTE, Flags::FLAG_Z))),
          il.Const(Sizes::BYTE, Flags::FLAG_Z))));
  il.AddInstruction(il.SetFlag(
      Flags::FLAG_C,
      il.And(Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
             il.Const(Sizes::BYTE, 1))));
  return true;
}

bool TaxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_X,
                     il.Register(Sizes::BYTE, Registers::HCS08_REG_A)));
  return true;
}

bool TpaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(
      il.SetRegister(Sizes::BYTE, Registers::HCS08_REG_A,
                     il.Register(Sizes::BYTE, Registers::HCS08_REG_CCR)));
  return true;
}

bool TstaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
                           il.Const(Sizes::BYTE, 0), Flags::FLAGS_Z_N_V));
  return true;
}

bool TstDir::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  uint8_t opr8a = opcode[1];  // opr8a is low byte of address in direct address
  il.AddInstruction(il.Sub(
      Sizes::BYTE, il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
      il.Const(Sizes::BYTE, 0), Flags::FLAGS_Z_N_V));
  return true;
}

bool TstIx::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                 BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE, IX_LOAD, il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));
  return true;
}

bool TstIx1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 = opcode[1];  // oprx8 is 8-bit offset from index register H:X
  il.AddInstruction(il.Sub(Sizes::BYTE, IX1_LOAD, il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));
  return true;
}

bool TstSp1::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  int16_t oprx8 =
      opcode[2];  // oprx8 operand is 8-bit offset from stack pointer
  il.AddInstruction(il.Sub(Sizes::BYTE, SP1_LOAD, il.Const(Sizes::BYTE, 0),
                           Flags::FLAGS_Z_N_V));
  return true;
}

bool TstxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.Sub(Sizes::BYTE,
                           il.Register(Sizes::BYTE, Registers::HCS08_REG_X),
                           il.Const(Sizes::BYTE, 0), Flags::FLAGS_Z_N_V));
  return true;
}

bool TsxInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_HX,
      il.Add(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_SP),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool TxaInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  il.AddInstruction(il.SetRegister(
      Sizes::BYTE, il.Register(Sizes::BYTE, Registers::HCS08_REG_A),
      il.Register(Sizes::BYTE, Registers::HCS08_REG_X)));
  return true;
}

bool TxsInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                  BN::LowLevelILFunction& il, BN::Architecture* arch) {
  len = length;
  // Load SP with contents of H:X (index reg) minus 1; H:X not altered
  il.AddInstruction(il.SetRegister(
      Sizes::WORD, Registers::HCS08_REG_SP,
      il.Sub(Sizes::WORD, il.Register(Sizes::WORD, Registers::HCS08_REG_HX),
             il.Const(Sizes::WORD, 1))));
  return true;
}

bool WaitInh::Lift(const uint8_t* opcode, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il, BN::Architecture* arch) {
  UNIMPLEMENTED
}

/* Helper Functions */
/** Conditional branch instruction
 *  REL (relative addressing)
 *  Format: <mnemonic> rel */
bool LiftBranchRel(const uint8_t* data, uint64_t addr, size_t& len,
                   BN::LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch) {
  uint8_t opcode = data[0];
  int8_t relOffset = data[1];  // rel operand is used to calculate branch target

  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget =
      falseBranchTarget + static_cast<int8_t>(relOffset);

  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);

  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;
  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }

  /* Determine branch instruction based on the condition code */
  BN::ExprId conditionIL;
  switch (opcode) {
    case Opcodes::BCC_BHS_REL:  // Carry bit clear/higher or same
      conditionIL = il.FlagCondition(LLFC_ULT);
      break;
    case Opcodes::BCS_BLO_REL:  // Carry bit set/lower
      conditionIL = il.FlagCondition(LLFC_UGE);
      break;
    case Opcodes::BEQ_REL:  // Equal
      conditionIL = il.FlagCondition(LLFC_E);
      break;
    case Opcodes::BGE_REL:  // Greater than or equal (signed)
      conditionIL = il.FlagCondition(LLFC_SGE);
      break;
    case Opcodes::BGT_REL:  // Greater than (signed)
      conditionIL = il.FlagCondition(LLFC_SGT);
      break;
    case Opcodes::BHI_REL:  // Higher
      conditionIL = il.FlagCondition(LLFC_UGT);
      break;
    case Opcodes::BLE_REL:  // Less than or equal to (signed)
      conditionIL = il.FlagCondition(LLFC_SLE);
      break;
    case Opcodes::BLS_REL:  // Lower or same
      conditionIL = il.FlagCondition(LLFC_ULE);
      break;
    case Opcodes::BLT_REL:  // Less than (signed)
      conditionIL = il.FlagCondition(LLFC_SLT);
      break;
    case Opcodes::BMI_REL:  // Minus, negative
      conditionIL = il.FlagCondition(LLFC_NEG);
      break;
    case Opcodes::BNE_REL:  // Not equal
      conditionIL = il.FlagCondition(LLFC_NE);
      break;
    case Opcodes::BPL_REL:  // Plus, positive
      conditionIL = il.FlagCondition(LLFC_POS);
      break;
    case Opcodes::BRA_REL:  // Always; unconditional branch
      il.AddInstruction(il.Jump(dest_if_true));
      return true;
    case Opcodes::BRN_REL:          // Branch never
      il.AddInstruction(il.Nop());  // Do nothing
      return true;
    case Opcodes::BHCC_REL:  // Half-carry bit clear
    case Opcodes::BHCS_REL:  // Half-carry bit set
    case Opcodes::BIH_REL:   // IRQ pin high
    case Opcodes::BIL_REL:   // IRQ pin low
    case Opcodes::BMC_REL:   // Interrupt mask clear
    case Opcodes::BMS_REL:   // Interrupt mask set
    default:
      il.AddInstruction(il.Unimplemented());
      return true;
  }

  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }

  il.AddInstruction(il.If(conditionIL, true_label, false_label));

  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }

  if (!found_false_label) {
    il.MarkLabel(false_label);
  }

  return true;
}

/** BSET/BCLR - Set/clear bit n in memory
 *  DIR (direct addressing)
 *  Format: <mnemonic> n,opr8a */
bool LiftBsetBclrDir(const uint8_t* data, size_t& len,
                     BN::LowLevelILFunction& il) {
  // Parse instruction fields
  uint8_t opcode = data[0];
  uint8_t bit_n = (opcode & 0xF) >> 1;  // Bit to set/clear
  uint8_t opr8a = data[1];              // Address (in direct addressing space
                                        // 0x0000-0x00FF) of byte to operate on

  // Determine which instruction based on least-significant bit
  if (opcode & 1) {  // BCLR - Clear bit N in memory
    uint8_t bitmask = ~(1 << bit_n);

    il.AddInstruction(il.Store(
        Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
        il.And(Sizes::BYTE,
               il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
               il.Const(Sizes::BYTE, bitmask))));
  } else {  // BSET - Set bit N in memory
    uint8_t bitmask = 1 << bit_n;

    il.AddInstruction(il.Store(
        Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a),
        il.Or(Sizes::BYTE,
              il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
              il.Const(Sizes::BYTE, bitmask))));
  }

  return true;
}

/** BRSET, BRCLR - branch if bit n in memory is set/clear
 *  DIR (direct) addressing
 *  Format: <mnemonic> n,opr8a,rel */
bool LiftBrsetBrclrDir(const uint8_t* data, uint64_t addr, size_t& len,
                       BN::LowLevelILFunction& il,
                       BinaryNinja::Architecture* arch) {
  // Parse instruction fields
  uint8_t opcode = data[0];
  uint8_t bit_n = opcode >> 1;  // Bit to set/clear
  uint8_t opr8a = data[1];      // Address (in direct addressing space
                                // 0x0000-0x00FF) of byte to operate on
  int8_t relOffset = data[2];   // Offset used to calculate branch target

  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }

  // Test bit in memory
  il.SetFlag(
      Flags::FLAG_C,
      il.TestBit(Sizes::BYTE,
                 il.Load(Sizes::BYTE, il.ConstPointer(Sizes::WORD, opr8a)),
                 il.Const(Sizes::BYTE, bit_n)));

  // Determine which instruction based on least-significant bit
  BN::ExprId conditionIL;
  if (opcode & 1) {  // BRCLR - Branch if bit N in memory is clear
    conditionIL = il.FlagCondition(LLFC_ULT);  // Carry bit clear

  } else {  // BRSET - Branch if bit N in memory is set
    conditionIL = il.FlagCondition(LLFC_UGE);  // Carry bit set
  }

  // Calculate branch targets
  uint16_t falseBranchTarget = addr + len;
  uint16_t trueBranchTarget = falseBranchTarget + signExtOffset;

  /* Get label for true branch, if one exists */
  BN::ExprId dest_if_true = il.Const(Sizes::WORD, trueBranchTarget);
  BNLowLevelILLabel *t, *f;
  t = il.GetLabelForAddress(arch, dest_if_true);

  bool indirect;
  BNLowLevelILLabel true_label, false_label;
  if (t) {
    indirect = false;
    true_label = *t;

  } else {
    indirect = true;
    true_label = BN::LowLevelILLabel();
  }

  BN::ExprId dest_if_false = il.Const(Sizes::WORD, falseBranchTarget);
  f = il.GetLabelForAddress(arch, dest_if_false);
  bool found_false_label;
  if (f) {
    found_false_label = true;
    false_label = *f;
  } else {
    found_false_label = false;
    false_label = BN::LowLevelILLabel();
  }

  il.AddInstruction(il.If(conditionIL, true_label, false_label));

  if (indirect) {
    il.MarkLabel(true_label);
    il.AddInstruction(il.Jump(dest_if_true));
  }

  if (!found_false_label) {
    il.MarkLabel(false_label);
  }

  return true;
}

}  // namespace HCS08