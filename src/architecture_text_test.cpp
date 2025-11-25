// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "architecture.h"
#include "opcodes.h"

namespace BN = BinaryNinja;

// range of ints to test IMM/EXT/etc. values
// TODO: can we somehow generate a list of X random numbers within a range?
// Some bugs only appear, for example, with values of 0xFF. It would be best
// to have a diverse set of integers to test with, but not all possible values
// as that takes way too long to compile/run.
constexpr int int_range_start = 0x0;
constexpr int int_range_end = 0x10;  // max: 0xff

// Convert an array of instruction text tokens to a string
static std::string tokens_to_string(
    const std::vector<BN::InstructionTextToken> &tokens) {
  std::string out;

  for (const auto &token : tokens) {
    out.append(token.text);
  }

  return out;
}

// Compare two text token vectors
static void compare_text_tokens(
    const std::vector<BN::InstructionTextToken> &a,
    const std::vector<BN::InstructionTextToken> &b) {
  EXPECT_EQ(a.size(), b.size());
  for (size_t i = 0; i < std::min(a.size(), b.size()); i++) {
    EXPECT_EQ(a[i].type, b[i].type)
        << "token types @ index " << i << " do not match";
  }

  const auto a_assembly = tokens_to_string(a);
  const auto b_assembly = tokens_to_string(b);

  EXPECT_EQ(a_assembly, b_assembly) << "accumulated token strings do not match";
}

// Verify that an architecture returns the correct text tokens
static void test_architecture_text(
    const uint8_t *opcode, const uint64_t address,
    const std::vector<BN::InstructionTextToken> &want) {
  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hsc08test");

  size_t len = 0;
  auto got = std::vector<BN::InstructionTextToken>{};

  EXPECT_TRUE(arch->GetInstructionText(opcode, address, len, got));
  compare_text_tokens(got, want);
}

// Format: INH (no operands, 1-byte opcode)
class TestTextInh : public ::testing::TestWithParam<int> {};

TEST_P(TestTextInh, TestText) {
  const uint8_t opcode = GetParam();
  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
  };

  test_architecture_text(&opcode, 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatInh, TestTextInh,
    ::testing::Values(HCS08::Opcodes::ASRA_INH, HCS08::Opcodes::ASRX_INH,
                      HCS08::Opcodes::BGND_INH, HCS08::Opcodes::CLC_INH,
                      HCS08::Opcodes::CLI_INH, HCS08::Opcodes::CLRA_INH,
                      HCS08::Opcodes::CLRX_INH, HCS08::Opcodes::CLRH_INH,
                      HCS08::Opcodes::COMA_INH, HCS08::Opcodes::COMX_INH,
                      HCS08::Opcodes::DAA_INH, HCS08::Opcodes::DECA_INH,
                      HCS08::Opcodes::DECX_INH, HCS08::Opcodes::DIV_INH,
                      HCS08::Opcodes::INCA_INH, HCS08::Opcodes::INCX_INH,
                      HCS08::Opcodes::LSLA_INH, HCS08::Opcodes::LSLX_INH,
                      HCS08::Opcodes::LSRA_INH, HCS08::Opcodes::LSRX_INH,
                      HCS08::Opcodes::MUL_INH, HCS08::Opcodes::NEGA_INH,
                      HCS08::Opcodes::NEGX_INH, HCS08::Opcodes::NOP_INH,
                      HCS08::Opcodes::NSA_INH, HCS08::Opcodes::PSHA_INH,
                      HCS08::Opcodes::PSHH_INH, HCS08::Opcodes::PSHX_INH,
                      HCS08::Opcodes::PULA_INH, HCS08::Opcodes::PULH_INH,
                      HCS08::Opcodes::PULX_INH, HCS08::Opcodes::ROLA_INH,
                      HCS08::Opcodes::ROLX_INH, HCS08::Opcodes::RORA_INH,
                      HCS08::Opcodes::RORX_INH, HCS08::Opcodes::RSP_INH,
                      HCS08::Opcodes::RTI_INH, HCS08::Opcodes::RTS_INH,
                      HCS08::Opcodes::SEC_INH, HCS08::Opcodes::SEI_INH,
                      HCS08::Opcodes::STOP_INH, HCS08::Opcodes::SWI_INH,
                      HCS08::Opcodes::TAP_INH, HCS08::Opcodes::TAX_INH,
                      HCS08::Opcodes::TPA_INH, HCS08::Opcodes::TSTA_INH,
                      HCS08::Opcodes::TSTX_INH, HCS08::Opcodes::TSX_INH,
                      HCS08::Opcodes::TXA_INH, HCS08::Opcodes::TXS_INH,
                      HCS08::Opcodes::WAIT_INH),
    [](const testing::TestParamInfo<TestTextInh::ParamType> &info) {
      const auto op = HCS08::Opcodes::NAMES.at(info.param);
      return "OP_" + op;
    });

// TODO: DBNZA and DBNZX are listed as INH format, but they have operands so
// they need to be tested elsewhere

// Format: IMM8 (8-bit immediate value, 1-byte opcode)
class TestTextImm8 : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestTextImm8, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t imm = std::get<1>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "#$" + fmt::format("{:x}", imm)},
  };
  const std::array<uint8_t, 2> full_op = {
      opcode,
      imm,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatImm8, TestTextImm8,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_IMM, HCS08::Opcodes::ADD_IMM,
                          HCS08::Opcodes::AIS_IMM, HCS08::Opcodes::AIX_IMM,
                          HCS08::Opcodes::AND_IMM, HCS08::Opcodes::BIT_IMM,
                          HCS08::Opcodes::CMP_IMM, HCS08::Opcodes::CPX_IMM,
                          HCS08::Opcodes::EOR_IMM, HCS08::Opcodes::LDA_IMM,
                          HCS08::Opcodes::LDX_IMM, HCS08::Opcodes::ORA_IMM,
                          HCS08::Opcodes::SBC_IMM, HCS08::Opcodes::SUB_IMM),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextImm8::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto imm = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_IMM_" + imm;
    });

// Format: IMM16 (16-bit immediate value, 1-byte opcode)
class TestTextImm16
    : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(TestTextImm16, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t imm_high = std::get<1>(GetParam());
  const uint8_t imm_low = std::get<2>(GetParam());
  const uint16_t imm16 = imm_high << 8 | imm_low;

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "#$" + fmt::format("{:x}", imm16)},
  };
  const std::array<uint8_t, 3> full_op = {
      opcode,
      imm_high,
      imm_low,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatImm16, TestTextImm16,
    ::testing::Combine(::testing::Values(HCS08::Opcodes::CPHX_IMM,
                                         HCS08::Opcodes::LDHX_IMM),
                       ::testing::Range(int_range_start, int_range_end),
                       ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextImm16::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto imm = std::to_string(std::get<1>(info.param));
      const uint8_t imm_high = std::get<1>(info.param);
      const uint8_t imm_low = std::get<2>(info.param);
      const uint16_t imm16 = imm_high << 8 | imm_low;

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_IMM_" + std::to_string(imm16);
    });

// Format: DIR (8-bit direct value, 1-byte opcode)
class TestTextDir : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestTextDir, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t dir = std::get<1>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {PossibleAddressToken, "$" + fmt::format("{:x}", dir)},
  };
  const std::array<uint8_t, 2> full_op = {
      opcode,
      dir,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatDir, TestTextDir,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_DIR, HCS08::Opcodes::ADD_DIR,
                          HCS08::Opcodes::AND_DIR, HCS08::Opcodes::ASR_DIR,
                          HCS08::Opcodes::BIT_DIR, HCS08::Opcodes::CLR_DIR,
                          HCS08::Opcodes::CMP_DIR, HCS08::Opcodes::COM_DIR,
                          HCS08::Opcodes::CPHX_DIR, HCS08::Opcodes::CPX_DIR,
                          HCS08::Opcodes::DEC_DIR, HCS08::Opcodes::EOR_DIR,
                          HCS08::Opcodes::INC_DIR, HCS08::Opcodes::JMP_DIR,
                          HCS08::Opcodes::JSR_DIR, HCS08::Opcodes::LDA_DIR,
                          HCS08::Opcodes::LDHX_DIR, HCS08::Opcodes::LDX_DIR,
                          HCS08::Opcodes::LSL_DIR, HCS08::Opcodes::LSR_DIR,
                          HCS08::Opcodes::NEG_DIR, HCS08::Opcodes::ORA_DIR,
                          HCS08::Opcodes::ROL_DIR, HCS08::Opcodes::ROR_DIR,
                          HCS08::Opcodes::SBC_DIR, HCS08::Opcodes::STA_DIR,
                          HCS08::Opcodes::STHX_DIR, HCS08::Opcodes::STX_DIR,
                          HCS08::Opcodes::SUB_DIR, HCS08::Opcodes::TST_DIR),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextDir::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto dir = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_DIR_" + dir;
    });

// Format: EXT (16-bit value, 1-byte opcode)
class TestTextExt : public ::testing::TestWithParam<std::tuple<int, int, int>> {
};

TEST_P(TestTextExt, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t opr16a_high = std::get<1>(GetParam());
  const uint8_t opr16a_low = std::get<2>(GetParam());
  const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {PossibleAddressToken, "$" + fmt::format("{:x}", opr16a)},
  };
  const std::array<uint8_t, 3> full_op = {
      opcode,
      opr16a_high,
      opr16a_low,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatExt, TestTextExt,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_EXT, HCS08::Opcodes::ADD_EXT,
                          HCS08::Opcodes::AND_EXT, HCS08::Opcodes::BIT_EXT,
                          HCS08::Opcodes::CMP_EXT, HCS08::Opcodes::CPHX_EXT,
                          HCS08::Opcodes::CPX_EXT, HCS08::Opcodes::EOR_EXT,
                          HCS08::Opcodes::JMP_EXT, HCS08::Opcodes::JSR_EXT,
                          HCS08::Opcodes::LDA_EXT, HCS08::Opcodes::LDHX_EXT,
                          HCS08::Opcodes::LDX_EXT, HCS08::Opcodes::ORA_EXT,
                          HCS08::Opcodes::SBC_EXT, HCS08::Opcodes::STA_EXT,
                          HCS08::Opcodes::STHX_EXT, HCS08::Opcodes::STX_EXT,
                          HCS08::Opcodes::SUB_EXT),
        ::testing::Range(int_range_start, int_range_end),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextExt::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto imm = std::to_string(std::get<1>(info.param));
      const uint8_t opr16a_high = std::get<1>(info.param);
      const uint8_t opr16a_low = std::get<2>(info.param);
      const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_EXT_" + std::to_string(opr16a);
    });

// Format: IX2 (16-bit value, 1-byte opcode)
class TestTextIx2 : public ::testing::TestWithParam<std::tuple<int, int, int>> {
};

TEST_P(TestTextIx2, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t opr16a_high = std::get<1>(GetParam());
  const uint8_t opr16a_low = std::get<2>(GetParam());
  const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "$" + fmt::format("{:x}", opr16a)},
      {OperandSeparatorToken, ","},
      {RegisterToken, "X"}};
  const std::array<uint8_t, 3> full_op = {
      opcode,
      opr16a_high,
      opr16a_low,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatIx2, TestTextIx2,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_IX2, HCS08::Opcodes::ADD_IX2,
                          HCS08::Opcodes::AND_IX2, HCS08::Opcodes::BIT_IX2,
                          HCS08::Opcodes::CMP_IX2, HCS08::Opcodes::CPX_IX2,
                          HCS08::Opcodes::EOR_IX2, HCS08::Opcodes::JMP_IX2,
                          HCS08::Opcodes::JSR_IX2, HCS08::Opcodes::LDA_IX2,
                          HCS08::Opcodes::LDX_IX2, HCS08::Opcodes::ORA_IX2,
                          HCS08::Opcodes::SBC_IX2, HCS08::Opcodes::STA_IX2,
                          HCS08::Opcodes::STX_IX2, HCS08::Opcodes::SUB_IX2),
        ::testing::Range(int_range_start, int_range_end),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextIx2::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const uint8_t opr16a_high = std::get<1>(info.param);
      const uint8_t opr16a_low = std::get<2>(info.param);
      const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_IX2_" + std::to_string(opr16a);
    });

// Format: IX1 (8-bit value, 1-byte opcode)
class TestTextIx1 : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestTextIx1, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t oprx8 = std::get<1>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "$" + fmt::format("{:x}", oprx8)},
      {OperandSeparatorToken, ","},
      {RegisterToken, "X"}};
  const std::array<uint8_t, 2> full_op = {
      opcode,
      oprx8,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatIx1, TestTextIx1,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_IX1, HCS08::Opcodes::ADD_IX1,
                          HCS08::Opcodes::AND_IX1, HCS08::Opcodes::ASR_IX1,
                          HCS08::Opcodes::BIT_IX1, HCS08::Opcodes::CLR_IX1,
                          HCS08::Opcodes::CMP_IX1, HCS08::Opcodes::COM_IX1,
                          HCS08::Opcodes::CPX_IX1, HCS08::Opcodes::DEC_IX1,
                          HCS08::Opcodes::EOR_IX1, HCS08::Opcodes::INC_IX1,
                          HCS08::Opcodes::JMP_IX1, HCS08::Opcodes::JSR_IX1,
                          HCS08::Opcodes::LDA_IX1, HCS08::Opcodes::LDX_IX1,
                          HCS08::Opcodes::LSL_IX1, HCS08::Opcodes::LSR_IX1,
                          HCS08::Opcodes::NEG_IX1, HCS08::Opcodes::ORA_IX1,
                          HCS08::Opcodes::ROL_IX1, HCS08::Opcodes::ROR_IX1,
                          HCS08::Opcodes::SBC_IX1, HCS08::Opcodes::STA_IX1,
                          HCS08::Opcodes::STX_IX1, HCS08::Opcodes::SUB_IX1,
                          HCS08::Opcodes::TST_IX1),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextIx1::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto oprx8 = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_IX1_" + oprx8;
    });

// Format: IX (no operands, 1-byte opcode)
class TestTextIx : public ::testing::TestWithParam<std::tuple<int>> {};

TEST_P(TestTextIx, TestText) {
  const uint8_t opcode = std::get<0>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {OperandSeparatorToken, ","},
      {RegisterToken, "X"}};
  const std::array<uint8_t, 2> full_op = {
      opcode,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatIx, TestTextIx,
    ::testing::Values(
        HCS08::Opcodes::ADC_IX, HCS08::Opcodes::ADD_IX, HCS08::Opcodes::AND_IX,
        HCS08::Opcodes::ASR_IX, HCS08::Opcodes::BIT_IX, HCS08::Opcodes::CLR_IX,
        HCS08::Opcodes::CMP_IX, HCS08::Opcodes::COM_IX, HCS08::Opcodes::CPX_IX,
        HCS08::Opcodes::DEC_IX, HCS08::Opcodes::EOR_IX, HCS08::Opcodes::INC_IX,
        HCS08::Opcodes::JMP_IX, HCS08::Opcodes::JSR_IX, HCS08::Opcodes::LDA_IX,
        HCS08::Opcodes::LDX_IX, HCS08::Opcodes::LSL_IX, HCS08::Opcodes::LSR_IX,
        HCS08::Opcodes::NEG_IX, HCS08::Opcodes::ORA_IX, HCS08::Opcodes::ROL_IX,
        HCS08::Opcodes::ROR_IX, HCS08::Opcodes::SBC_IX, HCS08::Opcodes::STA_IX,
        HCS08::Opcodes::STX_IX, HCS08::Opcodes::SUB_IX, HCS08::Opcodes::TST_IX),
    [](const testing::TestParamInfo<TestTextIx::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_IX";
    });

// Format: SP1-3byte (8-bit value, 2-byte opcode)
class TestTextSp1_3byte
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestTextSp1_3byte, TestText) {
  const uint16_t opcode = std::get<0>(GetParam());
  const uint8_t oprx8 = std::get<1>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "$" + fmt::format("{:x}", oprx8)},
      {OperandSeparatorToken, ","},
      {RegisterToken, "SP"}};

  const std::array<uint8_t, 3> full_op = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),  // separate 2-byte opcode
      static_cast<uint8_t>(opcode & 0xFF),
      oprx8,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatSp1_3byte, TestTextSp1_3byte,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_SP1, HCS08::Opcodes::ADD_SP1,
                          HCS08::Opcodes::AND_SP1, HCS08::Opcodes::ASR_SP1,
                          HCS08::Opcodes::BIT_SP1, HCS08::Opcodes::CLR_SP1,
                          HCS08::Opcodes::CMP_SP1, HCS08::Opcodes::COM_SP1,
                          HCS08::Opcodes::CPHX_SP1, HCS08::Opcodes::CPX_SP1,
                          HCS08::Opcodes::DEC_SP1, HCS08::Opcodes::EOR_SP1,
                          HCS08::Opcodes::INC_SP1, HCS08::Opcodes::LDA_SP1,
                          HCS08::Opcodes::LDHX_SP1, HCS08::Opcodes::LDX_SP1,
                          HCS08::Opcodes::LSL_SP1, HCS08::Opcodes::LSR_SP1,
                          HCS08::Opcodes::NEG_SP1, HCS08::Opcodes::ORA_SP1,
                          HCS08::Opcodes::ROL_SP1, HCS08::Opcodes::ROR_SP1,
                          HCS08::Opcodes::SBC_SP1, HCS08::Opcodes::STA_SP1,
                          HCS08::Opcodes::STHX_SP1, HCS08::Opcodes::STX_SP1,
                          HCS08::Opcodes::SUB_SP1, HCS08::Opcodes::TST_SP1),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextSp1_3byte::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto oprx8 = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_SP1_" + oprx8;
    });

// Format: SP1-4byte (8-bit value, 8-bit offset, 2-byte opcode)
class TestTextSp1_4byte
    : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(TestTextSp1_4byte, TestText) {
  const uint16_t opcode = std::get<0>(GetParam());
  const uint8_t oprx8 = std::get<1>(GetParam());
  const uint8_t rel = std::get<2>(GetParam());

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "$" + fmt::format("{:x}", oprx8)},
      {OperandSeparatorToken, ","},
      {RegisterToken, "SP"},
      {OperandSeparatorToken, ","},
      {IntegerToken, "$" + fmt::format("{:x}", rel)},
  };
  const std::array<uint8_t, 4> full_op = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),  // separate 2-byte opcode
      static_cast<uint8_t>(opcode & 0xFF),
      oprx8,
      rel,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatSp1_4byte, TestTextSp1_4byte,
    ::testing::Combine(::testing::Values(HCS08::Opcodes::CBEQ_SP1,
                                         HCS08::Opcodes::DBNZ_SP1),
                       ::testing::Range(int_range_start, int_range_end),
                       ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextSp1_4byte::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const uint8_t oprx8 = std::get<1>(info.param);
      const uint8_t rel = std::get<2>(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_SP1_" + std::to_string(oprx8) + "_REL_" +
             std::to_string(rel);
    });

// Format: SP2 (16-bit value, 2-byte opcode)
class TestTextSp2 : public ::testing::TestWithParam<std::tuple<int, int, int>> {
};

TEST_P(TestTextSp2, TestText) {
  const uint16_t opcode = std::get<0>(GetParam());
  const uint8_t opr16a_high = std::get<1>(GetParam());
  const uint8_t opr16a_low = std::get<2>(GetParam());
  const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

  const std::vector<BN::InstructionTextToken> want = {
      {InstructionToken, HCS08::Opcodes::NAMES.at(opcode)},
      {TextToken, " "},
      {IntegerToken, "$" + fmt::format("{:x}", opr16a)},
      {OperandSeparatorToken, ","},
      {RegisterToken, "SP"}};
  const std::array<uint8_t, 4> full_op = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),  // separate 2-byte opcode
      static_cast<uint8_t>(opcode & 0xFF),
      opr16a_high,
      opr16a_low,
  };
  test_architecture_text(full_op.data(), 0x0, want);
}

INSTANTIATE_TEST_SUITE_P(
    TestTextFormatSp2, TestTextSp2,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::ADC_SP2, HCS08::Opcodes::ADD_SP2,
                          HCS08::Opcodes::AND_SP2, HCS08::Opcodes::BIT_SP2,
                          HCS08::Opcodes::CMP_SP2, HCS08::Opcodes::CPX_SP2,
                          HCS08::Opcodes::EOR_SP2, HCS08::Opcodes::LDA_SP2,
                          HCS08::Opcodes::LDX_SP2, HCS08::Opcodes::ORA_SP2,
                          HCS08::Opcodes::SBC_SP2, HCS08::Opcodes::STA_SP2,
                          HCS08::Opcodes::STX_SP2, HCS08::Opcodes::SUB_SP2),
        ::testing::Range(int_range_start, int_range_end),
        ::testing::Range(int_range_start, int_range_end)),
    [](const testing::TestParamInfo<TestTextSp2::ParamType> &info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const uint8_t opr16a_high = std::get<1>(info.param);
      const uint8_t opr16a_low = std::get<2>(info.param);
      const uint16_t opr16a = opr16a_high << 8 | opr16a_low;

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_SP2_" + std::to_string(opr16a);
    });
