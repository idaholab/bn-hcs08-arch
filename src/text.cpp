// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "text.h"

#include <binaryninjaapi.h>

#include <cstddef>
#include <cstdint>

#include "instructions.h"
#include "opcodes.h"
#include "registers.h"
#include "sizes.h"

namespace BN = BinaryNinja;

#define ITEXT(m) result.emplace_back(InstructionToken, m);
#define SPACETEXT result.emplace_back(TextToken, " ");
#define REG_X_TEXT                        \
  result.emplace_back(RegisterToken, "X", \
                      Registers::HCS08_REG_HX);  // Index register H:X
#define REG_SP_TEXT \
  result.emplace_back(RegisterToken, "SP", Registers::HCS08_REG_SP);

#define OPR8I_FORMAT_STR \
  "#$%hhx"  // # indicates immediate, $ indicates hexadecimal; see pg. 188 of
            // HCS08 rer
#define OPR16I_FORMAT_STR "#$%hx"
#define OPR8A_FORMAT_STR \
  "$%hhx"  // $ indicates hexadecimal, is 8-bit address in direct address space
           // ($00xx)
#define OPR16A_FORMAT_STR \
  "$%hx"  // $ indicates hexadecimal, is 16-bit address anywhere in 64KB address
          // space
#define OPRX8_FORMAT_STR "$%hhx"
#define OPRX16_FORMAT_STR "$%hx"
#define REL_FORMAT_STR_2BYTE "$%hx"
#define REL_FORMAT_STR_BYTE "$%hhx"

namespace HCS08 {
/**
 * TextBrsetBrclr - Generate text for 3-byte BRSET and BRCLR instructions
 *      Branch if bit n in memory is (set | clear)
 * Format: <mnemonic> n,opr8a,rel
 */
bool TextBrsetBrclr(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  // Parse instruction fields
  uint8_t opcode = data[0];
  uint8_t opr8a = data[1];
  int8_t relOffset = data[2];

  // Sign-extend relative offset to 16-bit int
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }

  // Determine which instruction based on least-significant bit
  if (opcode & 1) {  // BRCLR - Branch if bit n in memory is clear
    ITEXT("brclr")
  } else {  // BRSET - Branch if bit n in memory is set
    ITEXT("brset")
  }
  SPACETEXT
  // Tokenize operands and generate text
  char buf[8];
  uint8_t bit_n = opcode >> 1;
  std::snprintf(buf, sizeof(buf), "%d", bit_n);  // Bit to test
  result.emplace_back(IntegerToken, buf, bit_n, sizeof(bit_n));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR,
                opr8a);  // Address in direct addressing space
  result.emplace_back(PossibleAddressToken, buf, opr8a, sizeof(opr8a));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_2BYTE,
                signExtOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, signExtOffset, sizeof(signExtOffset));
  return true;
}

/**
 * TextBsetBclr - Generate text for 2-byte BSET and BCLR instructions
 *      Set/clear bit n in memory
 * Format: <mnemonic> n,opr8a
 */
bool TextBsetBclr(const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  // Parse instruction fields
  uint8_t opcode = data[0];
  uint8_t opr8a = data[1];

  // Determine which instruction based on least-significant bit
  if (opcode & 1) {  // Clear bit n in memory
    ITEXT("bclr")
  } else {  // Set bit n in memory
    ITEXT("bset")
  }
  SPACETEXT
  // Tokenize operands and generate text
  char buf[8];
  uint8_t bit_n = (opcode & 0xF) >> 1;
  std::snprintf(buf, sizeof(buf), "%d", bit_n);  // Bit to test
  result.emplace_back(IntegerToken, buf, bit_n, sizeof(bit_n));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR,
                opr8a);  // Address in direct addressing space
  result.emplace_back(IntegerToken, buf, opr8a, sizeof(opr8a));

  return true;
}

/**
 * TextBranch0x2X - Text for 2-byte branch instructions, opcodes 0x2X
 * Format: <mnemonic> rel
 */
bool TextBranch0x2X(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  uint8_t opcode = data[0];
  uint8_t relOffset = data[1];

  switch (opcode) {
    case Opcodes::BRA_REL:
      ITEXT("bra")
      break;
    case Opcodes::BRN_REL:
      ITEXT("brn")
      break;
    case Opcodes::BHI_REL:
      ITEXT("bhi")
      break;
    case Opcodes::BLS_REL:
      ITEXT("bls")
      break;
    case Opcodes::BCC_BHS_REL:
      ITEXT("bcc")
      break;
    case Opcodes::BCS_BLO_REL:
      ITEXT("bcs")
      break;
    case Opcodes::BNE_REL:
      ITEXT("bne")
      break;
    case Opcodes::BEQ_REL:
      ITEXT("beq")
      break;
    case Opcodes::BHCC_REL:
      ITEXT("bhcc")
      break;
    case Opcodes::BHCS_REL:
      ITEXT("bhcs")
      break;
    case Opcodes::BPL_REL:
      ITEXT("bpl")
      break;
    case Opcodes::BMI_REL:
      ITEXT("bmi")
      break;
    case Opcodes::BMC_REL:
      ITEXT("bmc")
      break;
    case Opcodes::BMS_REL:
      ITEXT("bms")
      break;
    case Opcodes::BIL_REL:
      ITEXT("bil")
      break;
    case Opcodes::BIH_REL:
      ITEXT("bih")
      break;
    default:
      return false;
  }
  SPACETEXT
  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE, relOffset);
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));
  return true;
}

/**
 * TextBranch0x9X - Text for 2-byte branch instructions, opcodes 0x9X
 * Format: <mnemonic> rel
 */
bool TextBranch0x9X(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  uint8_t opcode = data[0];
  uint8_t relOffset = data[1];

  switch (opcode) {
    case Opcodes::BGE_REL:
      ITEXT("bge")
      break;
    case Opcodes::BLT_REL:
      ITEXT("blt")
      break;
    case Opcodes::BGT_REL:
      ITEXT("bgt")
      break;
    case Opcodes::BLE_REL:
      ITEXT("ble")
      break;
    default:
      return false;
  }
  SPACETEXT
  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE, relOffset);
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextBsr - Text for 2-byte BSR instruction, opcode 0xAD
 *      Branch to subroutine
 * Format: bsr rel
 */
bool TextBsr(const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  ITEXT("bsr")
  SPACETEXT

  int8_t relOffset = data[1];
  int16_t signExtOffset;
  if (relOffset >> 7) {  // Negative
    signExtOffset = 0xFF00 | relOffset;
  } else {  // Positive
    signExtOffset = relOffset;
  }
  signExtOffset = signExtOffset + len;

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_2BYTE, signExtOffset);
  result.emplace_back(IntegerToken, buf, signExtOffset, sizeof(signExtOffset));

  return true;
}

/**
 * TextCbeqaCbeqx - Generate text for 3-byte CBEQA and CBEQX instructions with
 * immediate (IMM) addressing Format: <mnemonic> #opr8i,rel
 */
bool TextCbeqaCbeqx(const char* mnemonic, const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand fields
  uint8_t opr8i = data[1];
  int8_t relOffset = data[2];

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8I_FORMAT_STR, opr8i);
  result.emplace_back(IntegerToken, buf, opr8i, sizeof(opr8i));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextIx1Plus3ByteCbeq - Generate text for 3-byte CBEQ instruction with IX1+
 * (indexed, 1-byte offset, with post increment) addressing Format: <mnemonic>
 * oprx8,X+,rel
 */
bool TextIx1Plus3ByteCbeq(const uint8_t* data, size_t& len,
                          std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("cbeq")
  SPACETEXT

  // Parse operand fields
  uint8_t oprx8 = data[1];
  int8_t relOffset = data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT
  result.emplace_back(TextToken, "+");

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextIxPlus2ByteCbeq - Generate text for 2-byte CBEQ instruction with IX+
 * (indexed, no offset, with post increment) addressing Format: <mnemonic>
 * ,X+,rel
 */
bool TextIxPlus2ByteCbeq(const uint8_t* data, size_t& len,
                         std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("cbeq")
  SPACETEXT

  // Parse operand fields
  int8_t relOffset = data[1];

  // Tokenize operands and generate disassembly text
  char buf[8];
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT
  result.emplace_back(TextToken, "+");

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextIx1Dbnz - Generate text for 3-byte DBNZ instruction with IX1 (indexed,
 * 8-bit offset) addressing Format: <mnemonic> oprx8,X,rel
 */
bool TextIx1Dbnz(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("cbeq")
  SPACETEXT

  // Parse operand fields
  uint8_t oprx8 = data[1];
  int8_t relOffset = data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextIxDbnz - Generate text for 2-byte DBNZ instruction with IX (indexed, no
 * offset) addressing Format: <mnemonic> ,X,rel
 */
bool TextIxDbnz(const uint8_t* data, size_t& len,
                std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("cbeq")
  SPACETEXT

  // Parse operand fields
  int8_t relOffset = data[1];

  // Tokenize operands and generate disassembly text
  char buf[8];
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextCphxLdhx - Generate text for 3-byte CPHX and LDHX instructions with
 * immediate (IMM) addressing Format: <mnemonic> #opr16i
 */
bool TextCphxLdhx(const char* mnemonic, const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand field
  uint16_t opr16i = data[1] << 8 | data[2];

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR16I_FORMAT_STR, opr16i);
  result.emplace_back(IntegerToken, buf, opr16i, sizeof(opr16i));

  return true;
}

/**
 * TextDir3ByteCbeqDbnz - Generate text for 3-byte CBEQ and DBNZ instructions
 * with direct (DIR) addressing Format: <mnemonic> opr8a,rel
 */
bool TextDir3ByteCbeqDbnz(const char* mnemonic, const uint8_t* data,
                          size_t& len,
                          std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand field
  uint8_t opr8a = data[1];
  uint8_t relOffset = data[2];

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR,
                opr8a);  // Address in direct addressing space
  result.emplace_back(PossibleAddressToken, buf, opr8a, sizeof(opr8a));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextInh2ByteDbnzaDbnzx - Generate text for 2-byte DBNZA and DBNZX
 * instructions with inherent addressing Format: <mnemonic> rel
 */
bool TextInh2ByteDbnzaDbnzx(const char* mnemonic, const int8_t relOffset,
                            size_t& len,
                            std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextImm - Generate text for 2-byte instructions with immediate addressing
 * Format: <mnemonic> #opr8i
 */
bool TextImm(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  int8_t opr8i = data[1];

  // Tokenize immediate operand and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8I_FORMAT_STR, opr8i);
  result.emplace_back(IntegerToken, buf, opr8i, sizeof(opr8i));

  return true;
}

/**
 * TextDir - Generate text for 2-byte instructions with direct addressing
 * Format: <mnemonic> opr8a
 */
bool TextDir2Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  uint8_t opr8a = data[1];

  // Tokenize 8-bit address operand opr8a and generate disassembly text
  // Note: opr8a is the low-order 8 bits of an addr in the direct address space
  // ($00xx)
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8a);
  result.emplace_back(PossibleAddressToken, buf, opr8a, sizeof(opr8a));

  return true;
}

/**
 * TextExt - Generate text for 3-byte instructions with extended addressing
 * Format: <mnemonic> opr16a
 */
bool TextExt(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  uint16_t opr16a = data[1] << 8 | data[2];

  // Tokenize 16-bit address operand opr16a and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR16A_FORMAT_STR, opr16a);
  result.emplace_back(PossibleAddressToken, buf, opr16a, sizeof(opr16a));

  return true;
}

/**
 * TextIx2 - Generate text for 3-byte instructions with IX2 addressing (indexed,
 * 16-bit offset) Format: <mnemonic> oprx16,X
 */
bool TextIx2(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  uint16_t oprx16 = data[1] << 8 | data[2];

  // Tokenize 16-bit offset operand oprx16 and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX16_FORMAT_STR, oprx16);
  result.emplace_back(IntegerToken, buf, oprx16, sizeof(oprx16));

  // Tokenize and generate text for register X, lower half of index register
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/**
 * TextIx1 - Generate text for 2-byte instructions with IX1 addressing (indexed,
 * 8-bit offset) Format: <mnemonic> oprx8,X
 */
bool TextIx1(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  uint8_t oprx8 = data[1];

  // Tokenize 8-bit offset operand oprx8 and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  // Tokenize and generate text for register X, lower half of index register
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/**
 * TextIx - Generate text for 1-byte instructions with IX addressing (indexed,
 * no offset) Format: <mnemonic> ,X
 */
bool TextIx(const char* mnemonic, size_t& len,
            std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Tokenize and generate text for register X, lower half of index register
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/**
 * TextInh - Generate text for 1-byte instructions with INH (inherent)
 * addressing Format: <mnemonic>
 */
bool TextInh(const char* mnemonic, size_t& len,
             std::vector<BN::InstructionTextToken>& result) {
  ITEXT(mnemonic)

  return true;
}

/**
 * TextMovDirDir - Generate text for 3-byte MOV instruction from DIR address to
 * DIR address Format: <mnemonic> opr8a,opr8a
 */
bool TextMovDirDir(const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("mov")
  SPACETEXT

  // Parse operand fields
  uint8_t opr8aSrc = data[1];
  uint8_t opr8aDst = data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8aSrc);
  result.emplace_back(IntegerToken, buf, opr8aSrc, sizeof(opr8aSrc));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8aDst);
  result.emplace_back(IntegerToken, buf, opr8aDst, sizeof(opr8aDst));

  return true;
}

/**
 * TextMovDirIxPlus - Generate text for 2-byte MOV instruction from DIR address
 * to IX+ (indexed, no offset, post increment) address Format: <mnemonic>
 * opr8a,X+
 */
bool TextMovDirIxPlus(const uint8_t* data, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("mov")
  SPACETEXT

  // Parse operand fields
  uint8_t opr8aSrc = data[1];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8aSrc);
  result.emplace_back(IntegerToken, buf, opr8aSrc, sizeof(opr8aSrc));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT
  result.emplace_back(TextToken, "+");

  return true;
}

/**
 * TextMovImmDir - Generate text for 3-byte MOV instruction from immediate (IMM)
 * value to direct (DIR) address Format: <mnemonic> #opr8i,opr8a
 */
bool TextMovImmDir(const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("mov")
  SPACETEXT

  // Parse operand fields
  uint8_t opr8iSrc = data[1];
  uint8_t opr8aDst = data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPR8I_FORMAT_STR, opr8iSrc);
  result.emplace_back(IntegerToken, buf, opr8iSrc, sizeof(opr8iSrc));

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8aDst);
  result.emplace_back(IntegerToken, buf, opr8aDst, sizeof(opr8aDst));

  return true;
}

/**
 * TextMovIxPlusDir - Generate text for 2-byte MOV instruction from IX+
 * (indexed, no offset, post increment) address to DIR (direct) address Format:
 * <mnemonic> ,X+,opr8a
 */
bool TextMovIxPostincDir(const uint8_t* data, size_t& len,
                         std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("mov")
  SPACETEXT

  // Parse operand fields
  uint8_t opr8aDst = data[1];

  // Tokenize operands and generate disassembly text
  char buf[8];
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT
  result.emplace_back(TextToken, "+");

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), OPR8A_FORMAT_STR, opr8aDst);
  result.emplace_back(IntegerToken, buf, opr8aDst, sizeof(opr8aDst));

  return true;
}

/**
 * TextSp1_3Byte - Generate text for 3-byte instructions with SP1 (stack
 * pointer, 8-bit offset) addressing Format: <mnemonic> oprx8,SP
 */
bool TextSp1_3Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand field
  uint8_t oprx8 = data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_SP_TEXT

  return true;
}

/**
 * TextSp1_4Byte - Generate text for 4-byte CBEQ and DBNZ instructions with SP1
 * (stack pointer, 8-bit offset) addressing Format: <mnemonic> oprx8,SP,rel
 */
bool TextSp1_4Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand field
  uint8_t oprx8 = data[2];
  int8_t relOffset = data[3];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_SP_TEXT

  result.emplace_back(OperandSeparatorToken, ",");
  std::snprintf(buf, sizeof(buf), REL_FORMAT_STR_BYTE,
                relOffset);  // Offset to calculate branch target
  result.emplace_back(IntegerToken, buf, relOffset, sizeof(relOffset));

  return true;
}

/**
 * TextSp2_4Byte - Generate text for 4-byte instructions with SP2 (stack
 * pointer, 8-bit offset) addressing Format: <mnemonic> oprx16,SP
 */
bool TextSp2_4Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT(mnemonic)
  SPACETEXT

  // Parse operand field
  uint16_t oprx16 = data[2] << 8 | data[3];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX16_FORMAT_STR, oprx16);
  result.emplace_back(IntegerToken, buf, oprx16, sizeof(oprx16));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_SP_TEXT

  return true;
}

/**
 * TextIx2Ldhx - Generate text for 4-byte LDHX instruction with IX2 (indexed,
 * 16-bit offset) addressing Format: <mnemonic> oprx16,X
 */
bool TextIx2Ldhx(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("ldhx")
  SPACETEXT

  // Parse operand fields
  uint16_t oprx16 = data[1] << 8 | data[2];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX16_FORMAT_STR, oprx16);
  result.emplace_back(IntegerToken, buf, oprx16, sizeof(oprx16));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/**
 * TextIx1Ldhx - Generate text for 3-byte LDHX instruction with IX1 (indexed,
 * 8-bit offset) addressing Format: <mnemonic> oprx8,X
 */
bool TextIx1Ldhx(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("ldhx")
  SPACETEXT

  // Parse operand fields
  uint8_t oprx8 = data[1];

  // Tokenize operands and generate disassembly text
  char buf[8];
  std::snprintf(buf, sizeof(buf), OPRX8_FORMAT_STR, oprx8);
  result.emplace_back(IntegerToken, buf, oprx8, sizeof(oprx8));

  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/**
 * TextIxLdhx - Generate text for 2-byte LDHX instruction with IX (indexed, no
 * offset) addressing Format: <mnemonic> ,X
 */
bool TextIxLdhx(size_t& len, std::vector<BN::InstructionTextToken>& result) {
  // Tokenize instruction mnemonic and generate disassembly text
  ITEXT("ldhx")
  SPACETEXT

  // Tokenize operands and generate disassembly text
  result.emplace_back(OperandSeparatorToken, ",");
  REG_X_TEXT

  return true;
}

/* Instruction Implementations */

bool AdcDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("adc", opcode, len, result);
}

bool AdcExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("adc", opcode, len, result);
}

bool AdcImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("adc", opcode, len, result);
}

bool AdcIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("adc", len, result);
}

bool AdcIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("adc", opcode, len, result);
}

bool AdcIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("adc", opcode, len, result);
}

bool AdcSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("adc", opcode, len, result);
}

bool AdcSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("adc", opcode, len, result);
}

bool AddDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("add", opcode, len, result);
}

bool AddExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("add", opcode, len, result);
}

bool AddImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("add", opcode, len, result);
}

bool AddIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("add", len, result);
}

bool AddIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("add", opcode, len, result);
}

bool AddIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("add", opcode, len, result);
}

bool AddSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("add", opcode, len, result);
}

bool AddSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("add", opcode, len, result);
}

bool AisImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("ais", opcode, len, result);
}

bool AixImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("aix", opcode, len, result);
}

bool AndDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("and", opcode, len, result);
}

bool AndExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("and", opcode, len, result);
}

bool AndImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("and", opcode, len, result);
}

bool AndIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("and", len, result);
}

bool AndIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("and", opcode, len, result);
}

bool AndIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("and", opcode, len, result);
}

bool AndSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("and", opcode, len, result);
}

bool AndSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("and", opcode, len, result);
}

bool AsraInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("asra", len, result);
}

bool AsrDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("asr", opcode, len, result);
}

bool AsrIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("asr", len, result);
}

bool AsrIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("asr", opcode, len, result);
}

bool AsrSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("asr", opcode, len, result);
}

bool AsrxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("asrx", len, result);
}

bool BccBhsRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BclrDirB0::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB3::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB4::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB5::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB6::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BclrDirB7::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BcsBloRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BeqRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BgeRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x9X(opcode, len, result);
}

bool BgndInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("bgnd", len, result);
}

bool BgtRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x9X(opcode, len, result);
}

bool BhccRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BhcsRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BhiRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BihRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BilRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BitDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("bit", opcode, len, result);
}

bool BitExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("bit", opcode, len, result);
}

bool BitImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("bit", opcode, len, result);
}

bool BitIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("bit", len, result);
}

bool BitIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("bit", opcode, len, result);
}

bool BitIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("bit", opcode, len, result);
}

bool BitSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("bit", opcode, len, result);
}

bool BitSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("bit", opcode, len, result);
}

bool BleRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x9X(opcode, len, result);
}

bool BltRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x9X(opcode, len, result);
}

bool BlsRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BmcRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BmiRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BmsRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BneRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BplRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BraRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BrclrDirB0::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB3::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB4::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB5::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB6::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrclrDirB7::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrnRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBranch0x2X(opcode, len, result);
}

bool BrsetDirB0::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB3::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB4::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB5::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB6::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BrsetDirB7::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                      std::vector<BN::InstructionTextToken>& result) {
  return TextBrsetBrclr(opcode, len, result);
}

bool BsetDirB0::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB3::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB4::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB5::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB6::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsetDirB7::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextBsetBclr(opcode, len, result);
}

bool BsrRel::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextBsr(opcode, len, result);
}

bool CbeqaImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  return TextCbeqaCbeqx("cbeqa", opcode, len, result);
}

bool CbeqDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextDir3ByteCbeqDbnz("cbeq", opcode, len, result);
}

bool CbeqIxPostinc::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                         std::vector<BN::InstructionTextToken>& result) {
  return TextIxPlus2ByteCbeq(opcode, len, result);
}

bool CbeqIx1Postinc::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                          std::vector<BN::InstructionTextToken>& result) {
  return TextIx1Plus3ByteCbeq(opcode, len, result);
}

bool CbeqxImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  return TextCbeqaCbeqx("cbeqx", opcode, len, result);
}

bool CbeqSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_4Byte("cbeq", opcode, len, result);
}

bool ClcInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("clc", len, result);
}

bool CliInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("cli", len, result);
}

bool ClraInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("clra", len, result);
}

bool ClrDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("clr", opcode, len, result);
}

bool ClrhInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("clrh", len, result);
}

bool ClrIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("clr", len, result);
}

bool ClrIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("clr", opcode, len, result);
}

bool ClrSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("clr", opcode, len, result);
}

bool ClrxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("clrx", len, result);
}

bool CmpDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("cmp", opcode, len, result);
}

bool CmpExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("cmp", opcode, len, result);
}

bool CmpImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("cmp", opcode, len, result);
}

bool CmpIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("cmp", len, result);
}

bool CmpIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("cmp", opcode, len, result);
}

bool CmpIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("cmp", opcode, len, result);
}

bool CmpSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("cmp", opcode, len, result);
}

bool CmpSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("cmp", opcode, len, result);
}

bool ComaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("coma", len, result);
}

bool ComDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("com", opcode, len, result);
}

bool ComIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("com", len, result);
}

bool ComIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("com", opcode, len, result);
}

bool ComSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("com", opcode, len, result);
}

bool ComxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("comx", len, result);
}

bool CphxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("cphx", opcode, len, result);
}

bool CphxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextExt("cphx", opcode, len, result);
}

bool CphxImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextCphxLdhx("cphx", opcode, len, result);
}

bool CphxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("cphx", opcode, len, result);
}

bool CpxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("cpx", opcode, len, result);
}

bool CpxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("cpx", opcode, len, result);
}

bool CpxImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("cpx", opcode, len, result);
}

bool CpxIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("cpx", len, result);
}

bool CpxIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("cpx", opcode, len, result);
}

bool CpxIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("cpx", opcode, len, result);
}

bool CpxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("cpx", opcode, len, result);
}

bool CpxSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("cpx", opcode, len, result);
}

bool DaaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("daa", len, result);
}

bool DbnzaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  return TextInh2ByteDbnzaDbnzx("dbnza", opcode[1], len, result);
}

bool DbnzDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextDir3ByteCbeqDbnz("dbnz", opcode, len, result);
}

bool DbnzIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIxDbnz(opcode, len, result);
}

bool DbnzIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextIx1Dbnz(opcode, len, result);
}

bool DbnzSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_4Byte("dbnz", opcode, len, result);
}

bool DbnzxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                    std::vector<BN::InstructionTextToken>& result) {
  return TextInh2ByteDbnzaDbnzx("dbnzx", opcode[1], len, result);
}

bool DecaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("deca", len, result);
}

bool DecDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("dec", opcode, len, result);
}

bool DecIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("dec", len, result);
}

bool DecIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("dec", opcode, len, result);
}

bool DecSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("dec", opcode, len, result);
}

bool DecxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("decx", len, result);
}

bool DivInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("div", len, result);
}

bool EorDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("eor", opcode, len, result);
}

bool EorExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("eor", opcode, len, result);
}

bool EorImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("eor", opcode, len, result);
}

bool EorIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("eor", len, result);
}

bool EorIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("eor", opcode, len, result);
}

bool EorIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("eor", opcode, len, result);
}

bool EorSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("eor", opcode, len, result);
}

bool EorSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("eor", opcode, len, result);
}

bool IncaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("inca", len, result);
}

bool IncDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("inc", opcode, len, result);
}

bool IncIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("inc", len, result);
}

bool IncIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("inc", opcode, len, result);
}

bool IncSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("inc", opcode, len, result);
}

bool IncxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("incx", len, result);
}

bool JmpDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("jmp", opcode, len, result);
}

bool JmpExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("jmp", opcode, len, result);
}

bool JmpIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("jmp", len, result);
}

bool JmpIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("jmp", opcode, len, result);
}

bool JmpIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("jmp", opcode, len, result);
}

bool JsrDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("jsr", opcode, len, result);
}

bool JsrExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("jsr", opcode, len, result);
}

bool JsrIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("jsr", len, result);
}

bool JsrIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("jsr", opcode, len, result);
}

bool JsrIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("jsr", opcode, len, result);
}

bool LdaDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("lda", opcode, len, result);
}

bool LdaExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("lda", opcode, len, result);
}

bool LdaImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("lda", opcode, len, result);
}

bool LdaIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("lda", len, result);
}

bool LdaIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("lda", opcode, len, result);
}

bool LdaIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("lda", opcode, len, result);
}

bool LdaSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("lda", opcode, len, result);
}

bool LdaSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("lda", opcode, len, result);
}

bool LdhxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("ldhx", opcode, len, result);
}

bool LdhxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextExt("ldhx", opcode, len, result);
}

bool LdhxImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextCphxLdhx("ldhx", opcode, len, result);
}

bool LdhxIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIxLdhx(len, result);
}

bool LdhxIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextIx1Ldhx(opcode, len, result);
}

bool LdhxIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextIx2Ldhx(opcode, len, result);
}

bool LdhxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("ldhx", opcode, len, result);
}

bool LdxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("ldx", opcode, len, result);
}

bool LdxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("ldx", opcode, len, result);
}

bool LdxImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("ldx", opcode, len, result);
}

bool LdxIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("ldx", len, result);
}

bool LdxIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("ldx", opcode, len, result);
}

bool LdxIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("ldx", opcode, len, result);
}

bool LdxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("ldx", opcode, len, result);
}

bool LdxSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("ldx", opcode, len, result);
}

bool LslaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("lsla", len, result);
}

bool LslDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("lsl", opcode, len, result);
}

bool LslIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("lsl", len, result);
}

bool LslIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("lsl", opcode, len, result);
}

bool LslSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("lsl", opcode, len, result);
}

bool LslxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("lslx", len, result);
}

bool LsraInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("lsra", len, result);
}

bool LsrDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("lsr", opcode, len, result);
}

bool LsrIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("lsr", len, result);
}

bool LsrIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("lsr", opcode, len, result);
}

bool LsrSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("lsr", opcode, len, result);
}

bool LsrxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("lsrx", len, result);
}

bool MovDirDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextMovDirDir(opcode, len, result);
}

bool MovDirIxPostinc::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                           std::vector<BN::InstructionTextToken>& result) {
  return TextMovDirIxPlus(opcode, len, result);
}

bool MovImmDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                     std::vector<BN::InstructionTextToken>& result) {
  return TextMovImmDir(opcode, len, result);
}

bool MovIxPostincDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                           std::vector<BN::InstructionTextToken>& result) {
  return TextMovIxPostincDir(opcode, len, result);
}

bool MulInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("mul", len, result);
}

bool NegaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("nega", len, result);
}

bool NegDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("neg", opcode, len, result);
}

bool NegIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("neg", len, result);
}

bool NegIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("neg", opcode, len, result);
}

bool NegSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("neg", opcode, len, result);
}

bool NegxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("negx", len, result);
}

bool NopInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("nop", len, result);
}

bool NsaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("nsa", len, result);
}

bool OraDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("ora", opcode, len, result);
}

bool OraExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("ora", opcode, len, result);
}

bool OraImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("ora", opcode, len, result);
}

bool OraIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("ora", len, result);
}

bool OraIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("ora", opcode, len, result);
}

bool OraIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("ora", opcode, len, result);
}

bool OraSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("ora", opcode, len, result);
}

bool OraSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("ora", opcode, len, result);
}

bool PshaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("psha", len, result);
}

bool PshhInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("pshh", len, result);
}

bool PshxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("pshx", len, result);
}

bool PulaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("pula", len, result);
}

bool PulhInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("pulh", len, result);
}

bool PulxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("pulx", len, result);
}

bool RolaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rola", len, result);
}

bool RolDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("rol", opcode, len, result);
}

bool RolIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("rol", len, result);
}

bool RolIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("rol", opcode, len, result);
}

bool RolSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("rol", opcode, len, result);
}

bool RolxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rolx", len, result);
}

bool RoraInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rora", len, result);
}

bool RorDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("ror", opcode, len, result);
}

bool RorIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("ror", len, result);
}

bool RorxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rorx", len, result);
}

bool RorIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("ror", opcode, len, result);
}

bool RorSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("ror", opcode, len, result);
}

bool RspInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rsp", len, result);
}

bool RtiInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rti", len, result);
}

bool RtsInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("rts", len, result);
}

bool SbcDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("sbc", opcode, len, result);
}

bool SbcExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("sbc", opcode, len, result);
}

bool SbcImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("sbc", opcode, len, result);
}

bool SbcIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("sbc", len, result);
}

bool SbcIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("sbc", opcode, len, result);
}

bool SbcIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("sbc", opcode, len, result);
}

bool SbcSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("sbc", opcode, len, result);
}

bool SbcSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("sbc", opcode, len, result);
}

bool SecInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("sec", len, result);
}

bool SeiInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("sei", len, result);
}

bool StaDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("sta", opcode, len, result);
}

bool StaExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("sta", opcode, len, result);
}

bool StaIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("sta", len, result);
}

bool StaIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("sta", opcode, len, result);
}

bool StaIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("sta", opcode, len, result);
}

bool StaSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("sta", opcode, len, result);
}

bool StaSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("sta", opcode, len, result);
}

bool SthxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("sthx", opcode, len, result);
}

bool SthxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextExt("sthx", opcode, len, result);
}

bool SthxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("sthx", opcode, len, result);
}

bool StopInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("stop", len, result);
}

bool StxDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("stx", opcode, len, result);
}

bool StxExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("stx", opcode, len, result);
}

bool StxIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("stx", len, result);
}

bool StxIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("stx", opcode, len, result);
}

bool StxIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("stx", opcode, len, result);
}

bool StxSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("stx", opcode, len, result);
}

bool StxSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("stx", opcode, len, result);
}

bool SubDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("sub", opcode, len, result);
}

bool SubExt::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextExt("sub", opcode, len, result);
}

bool SubImm::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextImm("sub", opcode, len, result);
}

bool SubIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("sub", len, result);
}

bool SubIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("sub", opcode, len, result);
}

bool SubIx2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx2("sub", opcode, len, result);
}

bool SubSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("sub", opcode, len, result);
}

bool SubSp2::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp2_4Byte("sub", opcode, len, result);
}

bool SwiInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("swi", len, result);
}

bool TapInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tap", len, result);
}

bool TaxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tax", len, result);
}

bool TpaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tpa", len, result);
}

bool TstaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tsta", len, result);
}

bool TstDir::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextDir2Byte("tst", opcode, len, result);
}

bool TstIx::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                 std::vector<BN::InstructionTextToken>& result) {
  return TextIx("tst", len, result);
}

bool TstIx1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextIx1("tst", opcode, len, result);
}

bool TstSp1::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextSp1_3Byte("tst", opcode, len, result);
}

bool TstxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tstx", len, result);
}

bool TsxInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("tsx", len, result);
}

bool TxaInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("txa", len, result);
}

bool TxsInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                  std::vector<BN::InstructionTextToken>& result) {
  return TextInh("txs", len, result);
}

bool WaitInh::Text(const uint8_t* opcode, uint64_t addr, size_t& len,
                   std::vector<BN::InstructionTextToken>& result) {
  return TextInh("wait", len, result);
}

}  // namespace HCS08
