// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "architecture.h"
#include "opcodes.h"

namespace BN = BinaryNinja;

// Test that all instructions are decoded
class TestInfoAllInstructions : public ::testing::TestWithParam<int> {};

TEST_P(TestInfoAllInstructions, TestInfo) {
  const uint16_t opcode = GetParam();

  const std::array<uint8_t, 2> bytes = {
      static_cast<uint8_t>((opcode & 0xFF00) >> 8),
      static_cast<uint8_t>(opcode & 0x00FF),
  };

  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hcs08test");
  auto result = BN::InstructionInfo{};

  EXPECT_TRUE(
      arch->GetInstructionInfo(bytes.data(), 0x0, bytes.size(), result));

  // decode the instruction again to ensure result.length is being set
  const auto map = HCS08::GenerateInstructionMap();
  const auto i = DecodeInstruction(bytes.data(), map);
  EXPECT_EQ(result.length, i->get()->GetLength());
}

// Test that all instructions are decoded
INSTANTIATE_TEST_SUITE_P(
    TestAll, TestInfoAllInstructions,
    ::testing::ValuesIn(HCS08::Opcodes::GetAllOpcodes()),
    [](const testing::TestParamInfo<TestInfoAllInstructions::ParamType>& info) {
      auto op = HCS08::Opcodes::NAMES.at(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      // append the opcode to keep test names unique
      return "OP_" + op + "_" + std::to_string(info.param);
    });

// Test non-branching instructions
class TestInfoNonBranchInstructions : public ::testing::TestWithParam<int> {};

TEST_P(TestInfoNonBranchInstructions, TestInfo) {
  std::array<uint8_t, 2> full_op;

  const uint16_t opcode = GetParam();

  // If this is a single-byte opcode, only create one byte
  if ((opcode & 0xFF00) >> 8 != HCS08::OpcodeFields::EXT_OP) {
    full_op = {
        static_cast<uint8_t>(opcode & 0xFF),
    };
  } else {
    full_op = {
        static_cast<uint8_t>((opcode & 0xFF00) >> 8),  // separate 2-byte opcode
        static_cast<uint8_t>(opcode & 0xFF),
    };
  }

  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hcs08test");
  auto result = BN::InstructionInfo{};

  arch->GetInstructionInfo(full_op.data(), 0x0, full_op.size(), result);
  EXPECT_EQ(result.branchCount, 0);
}

// Test non-branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestNonBranchInstructions, TestInfoNonBranchInstructions,
    ::testing::ValuesIn(HCS08::Opcodes::GetNonBranchingOpcodes()),
    [](const testing::TestParamInfo<TestInfoNonBranchInstructions::ParamType>&
           info) {
      auto op = HCS08::Opcodes::NAMES.at(info.param);

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_" + fmt::format("{:x}", info.param);
    });

// Test instruction info for conditional branch instructions
class TestInfoConditionalBranchInstructions
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestInfoConditionalBranchInstructions, TestInfo) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t offset = std::get<1>(GetParam());

  const std::array<uint8_t, 2> full_op = {
      opcode,
      offset,
  };

  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hcs08test");
  auto result = BN::InstructionInfo{};

  uint64_t addr = 0x1000;  // fake instruction address

  arch->GetInstructionInfo(full_op.data(), addr, full_op.size(), result);
  EXPECT_EQ(result.branchCount, 2);
  EXPECT_EQ(result.branchTarget[1], addr + result.length);
  EXPECT_EQ(result.branchTarget[0],
            addr + result.length + static_cast<int8_t>(offset));
  EXPECT_EQ(result.branchType[1], FalseBranch);
  EXPECT_EQ(result.branchType[0], TrueBranch);
  EXPECT_EQ(result.delaySlots, false);
}

// Test all conditional branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestConditionalBranches, TestInfoConditionalBranchInstructions,
    ::testing::Combine(
        ::testing::Values(HCS08::Opcodes::BEQ_REL, HCS08::Opcodes::BNE_REL,
                          HCS08::Opcodes::BCC_BHS_REL,
                          HCS08::Opcodes::BCS_BLO_REL, HCS08::Opcodes::BPL_REL,
                          HCS08::Opcodes::BMI_REL, HCS08::Opcodes::BIL_REL,
                          HCS08::Opcodes::BIH_REL, HCS08::Opcodes::BMC_REL,
                          HCS08::Opcodes::BMS_REL, HCS08::Opcodes::BHCC_REL,
                          HCS08::Opcodes::BHCS_REL, HCS08::Opcodes::BLT_REL,
                          HCS08::Opcodes::BLE_REL, HCS08::Opcodes::BGE_REL,
                          HCS08::Opcodes::BGT_REL, HCS08::Opcodes::BLS_REL,
                          HCS08::Opcodes::BHI_REL),
        ::testing::Range(0, 0xff)),
    [](const testing::TestParamInfo<
        TestInfoConditionalBranchInstructions::ParamType>& info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto offset = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_OFFSET_" + offset;
    });

// Test instruction info for unconditional branch instructions
class TestInfoUnconditionalBranchInstructions
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestInfoUnconditionalBranchInstructions, TestInfo) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t offset = std::get<1>(GetParam());

  const std::array<uint8_t, 2> full_op = {
      opcode,
      offset,
  };

  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hcs08test");
  auto result = BN::InstructionInfo{};

  uint64_t addr = 0x1000;  // fake instruction address

  arch->GetInstructionInfo(full_op.data(), addr, full_op.size(), result);
  EXPECT_EQ(result.branchCount, 1);
  EXPECT_EQ(result.branchTarget[0],
            addr + result.length + static_cast<int8_t>(offset));
  EXPECT_EQ(result.branchType[0], UnconditionalBranch);
  EXPECT_EQ(result.delaySlots, false);
}

// Test all unconditional branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestUnconditionalBranches, TestInfoUnconditionalBranchInstructions,
    ::testing::Combine(::testing::Values(HCS08::Opcodes::BRA_REL),
                       ::testing::Range(0, 0xff)),
    [](const testing::TestParamInfo<
        TestInfoUnconditionalBranchInstructions::ParamType>& info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto offset = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_OFFSET_" + offset;
    });

// Test instruction info for BRSET/BRCLR instructions
class TestInfoBrsetBrclrInstructions
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TestInfoBrsetBrclrInstructions, TestInfo) {
  const uint8_t opcode = std::get<0>(GetParam());
  const uint8_t rel = std::get<1>(GetParam());

  const std::array<uint8_t, 3> full_op = {
      opcode,
      0x0,  // mem location - does not affect target
      rel,
  };

  const auto arch = std::make_unique<HCS08::HCS08Architecture>("hcs08test");
  auto result = BN::InstructionInfo{};

  uint64_t addr = 0x1000;  // fake instruction address

  arch->GetInstructionInfo(full_op.data(), addr, full_op.size(), result);
  EXPECT_EQ(result.branchCount, 2);
  EXPECT_EQ(result.branchTarget[1], addr + result.length);
  EXPECT_EQ(result.branchTarget[0],
            addr + result.length + static_cast<int8_t>(rel));
  EXPECT_EQ(result.branchType[1], FalseBranch);
  EXPECT_EQ(result.branchType[0], TrueBranch);
  EXPECT_EQ(result.delaySlots, false);
}

// Test all conditional branching instructions
INSTANTIATE_TEST_SUITE_P(
    TestBrsetBrclrBranches, TestInfoBrsetBrclrInstructions,
    ::testing::Combine(
        ::testing::Values(
            HCS08::Opcodes::BRCLR_DIR_B0, HCS08::Opcodes::BRCLR_DIR_B1,
            HCS08::Opcodes::BRCLR_DIR_B2, HCS08::Opcodes::BRCLR_DIR_B3,
            HCS08::Opcodes::BRCLR_DIR_B4, HCS08::Opcodes::BRCLR_DIR_B5,
            HCS08::Opcodes::BRCLR_DIR_B6, HCS08::Opcodes::BRCLR_DIR_B7,
            HCS08::Opcodes::BRSET_DIR_B0, HCS08::Opcodes::BRSET_DIR_B1,
            HCS08::Opcodes::BRSET_DIR_B2, HCS08::Opcodes::BRSET_DIR_B3,
            HCS08::Opcodes::BRSET_DIR_B4, HCS08::Opcodes::BRSET_DIR_B5,
            HCS08::Opcodes::BRSET_DIR_B6, HCS08::Opcodes::BRSET_DIR_B7),
        ::testing::Range(0, 0xff)),
    [](const testing::TestParamInfo<TestInfoBrsetBrclrInstructions::ParamType>&
           info) {
      auto op = HCS08::Opcodes::NAMES.at(std::get<0>(info.param));
      const auto rel = std::to_string(std::get<1>(info.param));

      // Sanitize instruction name for printing
      op.erase(std::remove_if(op.begin(), op.end(),
                              [](char c) { return c == '/' || c == '.'; }),
               op.end());

      return "OP_" + op + "_" + fmt::format("{:x}", std::get<0>(info.param)) +
             "_REL_" + rel;
    });