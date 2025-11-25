// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef TEXT_H
#define TEXT_H

#include <binaryninjaapi.h>

#include <cstdint>
#include <vector>

namespace BN = BinaryNinja;

namespace HCS08 {
bool TextBrsetBrclr(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result);
bool TextBsetBclr(const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result);
bool TextBranch0x2X(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result);
bool TextBranch0x9X(const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result);
bool TextBsr(const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextCbeqaCbeqx(const char* mnemonic, const uint8_t* data, size_t& len,
                    std::vector<BN::InstructionTextToken>& result);
bool TextIx1Plus3ByteCbeq(const uint8_t* data, size_t& len,
                          std::vector<BN::InstructionTextToken>& result);
bool TextIxPlus2ByteCbeq(const uint8_t* data, size_t& len,
                         std::vector<BN::InstructionTextToken>& result);
bool TextIx1Dbnz(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result);
bool TextIxDbnz(const uint8_t* data, size_t& len,
                std::vector<BN::InstructionTextToken>& result);
bool TextCphxLdhx(const char* mnemonic, const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result);
bool TextDir3ByteCbeqDbnz(const char* mnemonic, const uint8_t* data,
                          size_t& len,
                          std::vector<BN::InstructionTextToken>& result);
bool TextInh2ByteDbnzaDbnzx(const char* mnemonic, int8_t relOffset, size_t& len,
                            std::vector<BN::InstructionTextToken>& result);
bool TextImm(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextDir2Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                  std::vector<BN::InstructionTextToken>& result);
bool TextExt(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextIx2(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextIx1(const char* mnemonic, const uint8_t* data, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextIx(const char* mnemonic, size_t& len,
            std::vector<BN::InstructionTextToken>& result);
bool TextInh(const char* mnemonic, size_t& len,
             std::vector<BN::InstructionTextToken>& result);
bool TextMovDirDir(const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result);
bool TextMovDirIxPlus(const uint8_t* data, size_t& len,
                      std::vector<BN::InstructionTextToken>& result);
bool TextMovImmDir(const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result);
bool TextMovIxPostincDir(const uint8_t* data, size_t& len,
                         std::vector<BN::InstructionTextToken>& result);
bool TextSp1_3Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result);
bool TextSp1_4Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result);
bool TextSp2_4Byte(const char* mnemonic, const uint8_t* data, size_t& len,
                   std::vector<BN::InstructionTextToken>& result);
bool TextIx2Ldhx(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result);
bool TextIx1Ldhx(const uint8_t* data, size_t& len,
                 std::vector<BN::InstructionTextToken>& result);
bool TextIxLdhx(size_t& len, std::vector<BN::InstructionTextToken>& result);
}  // namespace HCS08
#endif  // TEXT_H
