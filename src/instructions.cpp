// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "instructions.h"

#include "opcodes.h"

namespace HCS08 {
Instruction::Instruction() {}

size_t Instruction1Byte::GetLength() { return length; }

size_t Instruction2Byte::GetLength() { return length; }

size_t Instruction3Byte::GetLength() { return length; }

size_t Instruction4Byte::GetLength() { return length; }

/**
 * DecodeInstruction parses an opcode and returns it's corresponding
 * Instruction object
 * @param opcode pointer to opcode data
 * @param instr_map pre-generated map of opcodes to shared Instruction object
 * pointers
 * @return a pointer to the Instruction object corresponding to the given
 * opcode, or nullopt if it was not a valid opcode
 */
std::optional<std::shared_ptr<Instruction>> DecodeInstruction(
    const uint8_t* opcode,
    std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
        instr_map) {
  // opcodes starting with 0x9E are extended
  if (opcode[0] == OpcodeFields::EXT_OP) {
    const uint16_t full_op = opcode[0] << 8 | opcode[1];
    if (instr_map.count(full_op)) {
      return instr_map[full_op];
    }
    return std::nullopt;
  }
  if (instr_map.count(opcode[0])) {
    return instr_map[opcode[0]];
  }
  return std::nullopt;
}

/*
 * Concrete instruction class constructors
 */
AdcImm::AdcImm() : Instruction2Byte() {}
AdcDir::AdcDir() : Instruction2Byte() {}
AdcExt::AdcExt() : Instruction3Byte() {}
AdcIx2::AdcIx2() : Instruction3Byte() {}
AdcIx1::AdcIx1() : Instruction2Byte() {}
AdcIx::AdcIx() : Instruction1Byte() {}
AdcSp2::AdcSp2() : Instruction4Byte() {}
AdcSp1::AdcSp1() : Instruction3Byte() {}
AddImm::AddImm() : Instruction2Byte() {}
AddDir::AddDir() : Instruction2Byte() {}
AddExt::AddExt() : Instruction3Byte() {}
AddIx2::AddIx2() : Instruction3Byte() {}
AddIx1::AddIx1() : Instruction2Byte() {}
AddIx::AddIx() : Instruction1Byte() {}
AddSp2::AddSp2() : Instruction4Byte() {}
AddSp1::AddSp1() : Instruction3Byte() {}
AisImm::AisImm() : Instruction2Byte() {}
AixImm::AixImm() : Instruction2Byte() {}
AndImm::AndImm() : Instruction2Byte() {}
AndDir::AndDir() : Instruction2Byte() {}
AndExt::AndExt() : Instruction3Byte() {}
AndIx2::AndIx2() : Instruction3Byte() {}
AndIx1::AndIx1() : Instruction2Byte() {}
AndIx::AndIx() : Instruction1Byte() {}
AndSp2::AndSp2() : Instruction4Byte() {}
AndSp1::AndSp1() : Instruction3Byte() {}
AsrDir::AsrDir() : Instruction2Byte() {}
AsraInh::AsraInh() : Instruction1Byte() {}
AsrxInh::AsrxInh() : Instruction1Byte() {}
AsrIx1::AsrIx1() : Instruction2Byte() {}
AsrIx::AsrIx() : Instruction1Byte() {}
AsrSp1::AsrSp1() : Instruction3Byte() {}
BccBhsRel::BccBhsRel() : Instruction2Byte() {}
BclrDirB0::BclrDirB0() : Instruction2Byte() {}
BclrDirB1::BclrDirB1() : Instruction2Byte() {}
BclrDirB2::BclrDirB2() : Instruction2Byte() {}
BclrDirB3::BclrDirB3() : Instruction2Byte() {}
BclrDirB4::BclrDirB4() : Instruction2Byte() {}
BclrDirB5::BclrDirB5() : Instruction2Byte() {}
BclrDirB6::BclrDirB6() : Instruction2Byte() {}
BclrDirB7::BclrDirB7() : Instruction2Byte() {}
BcsBloRel::BcsBloRel() : Instruction2Byte() {}
BeqRel::BeqRel() : Instruction2Byte() {}
BgeRel::BgeRel() : Instruction2Byte() {}
BgndInh::BgndInh() : Instruction1Byte() {}
BgtRel::BgtRel() : Instruction2Byte() {}
BhccRel::BhccRel() : Instruction2Byte() {}
BhcsRel::BhcsRel() : Instruction2Byte() {}
BhiRel::BhiRel() : Instruction2Byte() {}
BihRel::BihRel() : Instruction2Byte() {}
BilRel::BilRel() : Instruction2Byte() {}
BitImm::BitImm() : Instruction2Byte() {}
BitDir::BitDir() : Instruction2Byte() {}
BitExt::BitExt() : Instruction3Byte() {}
BitIx2::BitIx2() : Instruction3Byte() {}
BitIx1::BitIx1() : Instruction2Byte() {}
BitIx::BitIx() : Instruction1Byte() {}
BitSp2::BitSp2() : Instruction4Byte() {}
BitSp1::BitSp1() : Instruction3Byte() {}
BleRel::BleRel() : Instruction2Byte() {}
BlsRel::BlsRel() : Instruction2Byte() {}
BltRel::BltRel() : Instruction2Byte() {}
BmcRel::BmcRel() : Instruction2Byte() {}
BmiRel::BmiRel() : Instruction2Byte() {}
BmsRel::BmsRel() : Instruction2Byte() {}
BneRel::BneRel() : Instruction2Byte() {}
BplRel::BplRel() : Instruction2Byte() {}
BraRel::BraRel() : Instruction2Byte() {}
BrclrDirB0::BrclrDirB0() : Instruction3Byte() {}
BrclrDirB1::BrclrDirB1() : Instruction3Byte() {}
BrclrDirB2::BrclrDirB2() : Instruction3Byte() {}
BrclrDirB3::BrclrDirB3() : Instruction3Byte() {}
BrclrDirB4::BrclrDirB4() : Instruction3Byte() {}
BrclrDirB5::BrclrDirB5() : Instruction3Byte() {}
BrclrDirB6::BrclrDirB6() : Instruction3Byte() {}
BrclrDirB7::BrclrDirB7() : Instruction3Byte() {}
BrnRel::BrnRel() : Instruction3Byte() {}
BrsetDirB0::BrsetDirB0() : Instruction3Byte() {}
BrsetDirB1::BrsetDirB1() : Instruction3Byte() {}
BrsetDirB2::BrsetDirB2() : Instruction3Byte() {}
BrsetDirB3::BrsetDirB3() : Instruction3Byte() {}
BrsetDirB4::BrsetDirB4() : Instruction3Byte() {}
BrsetDirB5::BrsetDirB5() : Instruction3Byte() {}
BrsetDirB6::BrsetDirB6() : Instruction3Byte() {}
BrsetDirB7::BrsetDirB7() : Instruction3Byte() {}
BsetDirB0::BsetDirB0() : Instruction2Byte() {}
BsetDirB1::BsetDirB1() : Instruction2Byte() {}
BsetDirB2::BsetDirB2() : Instruction2Byte() {}
BsetDirB3::BsetDirB3() : Instruction2Byte() {}
BsetDirB4::BsetDirB4() : Instruction2Byte() {}
BsetDirB5::BsetDirB5() : Instruction2Byte() {}
BsetDirB6::BsetDirB6() : Instruction2Byte() {}
BsetDirB7::BsetDirB7() : Instruction2Byte() {}
BsrRel::BsrRel() : Instruction2Byte() {}
CbeqDir::CbeqDir() : Instruction3Byte() {}
CbeqaImm::CbeqaImm() : Instruction3Byte() {}
CbeqxImm::CbeqxImm() : Instruction3Byte() {}
CbeqIx1Postinc::CbeqIx1Postinc() : Instruction3Byte() {}
CbeqIxPostinc::CbeqIxPostinc() : Instruction2Byte() {}
CbeqSp1::CbeqSp1() : Instruction4Byte() {}
ClcInh::ClcInh() : Instruction1Byte() {}
CliInh::CliInh() : Instruction1Byte() {}
ClrDir::ClrDir() : Instruction2Byte() {}
ClraInh::ClraInh() : Instruction1Byte() {}
ClrxInh::ClrxInh() : Instruction1Byte() {}
ClrhInh::ClrhInh() : Instruction1Byte() {}
ClrIx1::ClrIx1() : Instruction2Byte() {}
ClrIx::ClrIx() : Instruction1Byte() {}
ClrSp1::ClrSp1() : Instruction3Byte() {}
CmpImm::CmpImm() : Instruction2Byte() {}
CmpDir::CmpDir() : Instruction2Byte() {}
CmpExt::CmpExt() : Instruction3Byte() {}
CmpIx2::CmpIx2() : Instruction3Byte() {}
CmpIx1::CmpIx1() : Instruction2Byte() {}
CmpIx::CmpIx() : Instruction1Byte() {}
CmpSp2::CmpSp2() : Instruction4Byte() {}
CmpSp1::CmpSp1() : Instruction3Byte() {}
ComDir::ComDir() : Instruction2Byte() {}
ComaInh::ComaInh() : Instruction1Byte() {}
ComxInh::ComxInh() : Instruction1Byte() {}
ComIx1::ComIx1() : Instruction2Byte() {}
ComIx::ComIx() : Instruction1Byte() {}
ComSp1::ComSp1() : Instruction3Byte() {}
CphxExt::CphxExt() : Instruction3Byte() {}
CphxImm::CphxImm() : Instruction3Byte() {}
CphxDir::CphxDir() : Instruction2Byte() {}
CphxSp1::CphxSp1() : Instruction3Byte() {}
CpxImm::CpxImm() : Instruction2Byte() {}
CpxDir::CpxDir() : Instruction2Byte() {}
CpxExt::CpxExt() : Instruction3Byte() {}
CpxIx2::CpxIx2() : Instruction3Byte() {}
CpxIx1::CpxIx1() : Instruction2Byte() {}
CpxIx::CpxIx() : Instruction1Byte() {}
CpxSp2::CpxSp2() : Instruction4Byte() {}
CpxSp1::CpxSp1() : Instruction3Byte() {}
DaaInh::DaaInh() : Instruction1Byte() {}
DbnzDir::DbnzDir() : Instruction3Byte() {}
DbnzaInh::DbnzaInh() : Instruction2Byte() {}
DbnzxInh::DbnzxInh() : Instruction2Byte() {}
DbnzIx1::DbnzIx1() : Instruction3Byte() {}
DbnzIx::DbnzIx() : Instruction2Byte() {}
DbnzSp1::DbnzSp1() : Instruction4Byte() {}
DecDir::DecDir() : Instruction2Byte() {}
DecaInh::DecaInh() : Instruction1Byte() {}
DecxInh::DecxInh() : Instruction1Byte() {}
DecIx1::DecIx1() : Instruction2Byte() {}
DecIx::DecIx() : Instruction1Byte() {}
DecSp1::DecSp1() : Instruction3Byte() {}
DivInh::DivInh() : Instruction1Byte() {}
EorImm::EorImm() : Instruction2Byte() {}
EorDir::EorDir() : Instruction2Byte() {}
EorExt::EorExt() : Instruction3Byte() {}
EorIx2::EorIx2() : Instruction3Byte() {}
EorIx1::EorIx1() : Instruction2Byte() {}
EorIx::EorIx() : Instruction1Byte() {}
EorSp2::EorSp2() : Instruction4Byte() {}
EorSp1::EorSp1() : Instruction3Byte() {}
IncDir::IncDir() : Instruction2Byte() {}
IncaInh::IncaInh() : Instruction1Byte() {}
IncxInh::IncxInh() : Instruction1Byte() {}
IncIx1::IncIx1() : Instruction2Byte() {}
IncIx::IncIx() : Instruction1Byte() {}
IncSp1::IncSp1() : Instruction3Byte() {}
JmpDir::JmpDir() : Instruction2Byte() {}
JmpExt::JmpExt() : Instruction3Byte() {}
JmpIx2::JmpIx2() : Instruction3Byte() {}
JmpIx1::JmpIx1() : Instruction2Byte() {}
JmpIx::JmpIx() : Instruction1Byte() {}
JsrDir::JsrDir() : Instruction2Byte() {}
JsrExt::JsrExt() : Instruction3Byte() {}
JsrIx2::JsrIx2() : Instruction3Byte() {}
JsrIx1::JsrIx1() : Instruction2Byte() {}
JsrIx::JsrIx() : Instruction1Byte() {}
LdaImm::LdaImm() : Instruction2Byte() {}
LdaDir::LdaDir() : Instruction2Byte() {}
LdaExt::LdaExt() : Instruction3Byte() {}
LdaIx2::LdaIx2() : Instruction3Byte() {}
LdaIx1::LdaIx1() : Instruction2Byte() {}
LdaIx::LdaIx() : Instruction1Byte() {}
LdaSp2::LdaSp2() : Instruction4Byte() {}
LdaSp1::LdaSp1() : Instruction3Byte() {}
LdhxImm::LdhxImm() : Instruction3Byte() {}
LdhxDir::LdhxDir() : Instruction2Byte() {}
LdhxExt::LdhxExt() : Instruction3Byte() {}
LdhxIx::LdhxIx() : Instruction2Byte() {}
LdhxIx2::LdhxIx2() : Instruction4Byte() {}
LdhxIx1::LdhxIx1() : Instruction3Byte() {}
LdhxSp1::LdhxSp1() : Instruction3Byte() {}
LdxImm::LdxImm() : Instruction2Byte() {}
LdxDir::LdxDir() : Instruction2Byte() {}
LdxExt::LdxExt() : Instruction3Byte() {}
LdxIx2::LdxIx2() : Instruction3Byte() {}
LdxIx1::LdxIx1() : Instruction2Byte() {}
LdxIx::LdxIx() : Instruction1Byte() {}
LdxSp2::LdxSp2() : Instruction4Byte() {}
LdxSp1::LdxSp1() : Instruction3Byte() {}
LslDir::LslDir() : Instruction2Byte() {}
LslaInh::LslaInh() : Instruction1Byte() {}
LslxInh::LslxInh() : Instruction1Byte() {}
LslIx1::LslIx1() : Instruction2Byte() {}
LslIx::LslIx() : Instruction1Byte() {}
LslSp1::LslSp1() : Instruction3Byte() {}
LsrDir::LsrDir() : Instruction2Byte() {}
LsraInh::LsraInh() : Instruction1Byte() {}
LsrxInh::LsrxInh() : Instruction1Byte() {}
LsrIx1::LsrIx1() : Instruction2Byte() {}
LsrIx::LsrIx() : Instruction1Byte() {}
LsrSp1::LsrSp1() : Instruction3Byte() {}
MovDirDir::MovDirDir() : Instruction3Byte() {}
MovDirIxPostinc::MovDirIxPostinc() : Instruction2Byte() {}
MovImmDir::MovImmDir() : Instruction3Byte() {}
MovIxPostincDir::MovIxPostincDir() : Instruction2Byte() {}
MulInh::MulInh() : Instruction1Byte() {}
NegDir::NegDir() : Instruction2Byte() {}
NegaInh::NegaInh() : Instruction1Byte() {}
NegxInh::NegxInh() : Instruction1Byte() {}
NegIx1::NegIx1() : Instruction2Byte() {}
NegIx::NegIx() : Instruction1Byte() {}
NegSp1::NegSp1() : Instruction3Byte() {}
NopInh::NopInh() : Instruction1Byte() {}
NsaInh::NsaInh() : Instruction1Byte() {}
OraImm::OraImm() : Instruction2Byte() {}
OraDir::OraDir() : Instruction2Byte() {}
OraExt::OraExt() : Instruction3Byte() {}
OraIx2::OraIx2() : Instruction3Byte() {}
OraIx1::OraIx1() : Instruction2Byte() {}
OraIx::OraIx() : Instruction1Byte() {}
OraSp2::OraSp2() : Instruction4Byte() {}
OraSp1::OraSp1() : Instruction3Byte() {}
PshaInh::PshaInh() : Instruction1Byte() {}
PshhInh::PshhInh() : Instruction1Byte() {}
PshxInh::PshxInh() : Instruction1Byte() {}
PulaInh::PulaInh() : Instruction1Byte() {}
PulhInh::PulhInh() : Instruction1Byte() {}
PulxInh::PulxInh() : Instruction1Byte() {}
RolDir::RolDir() : Instruction2Byte() {}
RolaInh::RolaInh() : Instruction1Byte() {}
RolxInh::RolxInh() : Instruction1Byte() {}
RolIx1::RolIx1() : Instruction2Byte() {}
RolIx::RolIx() : Instruction1Byte() {}
RolSp1::RolSp1() : Instruction3Byte() {}
RorDir::RorDir() : Instruction2Byte() {}
RoraInh::RoraInh() : Instruction1Byte() {}
RorxInh::RorxInh() : Instruction1Byte() {}
RorIx1::RorIx1() : Instruction2Byte() {}
RorIx::RorIx() : Instruction1Byte() {}
RorSp1::RorSp1() : Instruction3Byte() {}
RspInh::RspInh() : Instruction1Byte() {}
RtiInh::RtiInh() : Instruction1Byte() {}
RtsInh::RtsInh() : Instruction1Byte() {}
SbcImm::SbcImm() : Instruction2Byte() {}
SbcDir::SbcDir() : Instruction2Byte() {}
SbcExt::SbcExt() : Instruction3Byte() {}
SbcIx2::SbcIx2() : Instruction3Byte() {}
SbcIx1::SbcIx1() : Instruction2Byte() {}
SbcIx::SbcIx() : Instruction1Byte() {}
SbcSp2::SbcSp2() : Instruction4Byte() {}
SbcSp1::SbcSp1() : Instruction3Byte() {}
SecInh::SecInh() : Instruction1Byte() {}
SeiInh::SeiInh() : Instruction1Byte() {}
StaDir::StaDir() : Instruction2Byte() {}
StaExt::StaExt() : Instruction3Byte() {}
StaIx2::StaIx2() : Instruction3Byte() {}
StaIx1::StaIx1() : Instruction2Byte() {}
StaIx::StaIx() : Instruction1Byte() {}
StaSp2::StaSp2() : Instruction4Byte() {}
StaSp1::StaSp1() : Instruction3Byte() {}
SthxDir::SthxDir() : Instruction2Byte() {}
SthxExt::SthxExt() : Instruction3Byte() {}
SthxSp1::SthxSp1() : Instruction3Byte() {}
StopInh::StopInh() : Instruction1Byte() {}
StxDir::StxDir() : Instruction2Byte() {}
StxExt::StxExt() : Instruction3Byte() {}
StxIx2::StxIx2() : Instruction3Byte() {}
StxIx1::StxIx1() : Instruction2Byte() {}
StxIx::StxIx() : Instruction1Byte() {}
StxSp2::StxSp2() : Instruction4Byte() {}
StxSp1::StxSp1() : Instruction3Byte() {}
SubImm::SubImm() : Instruction2Byte() {}
SubDir::SubDir() : Instruction2Byte() {}
SubExt::SubExt() : Instruction3Byte() {}
SubIx2::SubIx2() : Instruction3Byte() {}
SubIx1::SubIx1() : Instruction2Byte() {}
SubIx::SubIx() : Instruction1Byte() {}
SubSp2::SubSp2() : Instruction4Byte() {}
SubSp1::SubSp1() : Instruction3Byte() {}
SwiInh::SwiInh() : Instruction1Byte() {}
TapInh::TapInh() : Instruction1Byte() {}
TaxInh::TaxInh() : Instruction1Byte() {}
TpaInh::TpaInh() : Instruction1Byte() {}
TstDir::TstDir() : Instruction2Byte() {}
TstaInh::TstaInh() : Instruction1Byte() {}
TstxInh::TstxInh() : Instruction1Byte() {}
TstIx1::TstIx1() : Instruction2Byte() {}
TstIx::TstIx() : Instruction1Byte() {}
TstSp1::TstSp1() : Instruction3Byte() {}
TsxInh::TsxInh() : Instruction1Byte() {}
TxaInh::TxaInh() : Instruction1Byte() {}
TxsInh::TxsInh() : Instruction1Byte() {}
WaitInh::WaitInh() : Instruction1Byte() {}

/**
 * GenerateInstructionMap generates a map of opcodes to Instructions.
 * This function should be called once when the HCS08Architecture is constructed
 * and the map should be cleared when it's destructed.
 * @return a map of opcodes to Instruction objects
 */
std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
GenerateInstructionMap() {
  std::unordered_map<uint16_t, std::optional<std::shared_ptr<Instruction>>>
      map = {
          {Opcodes::ADC_IMM, std::make_shared<AdcImm>()},
          {Opcodes::ADC_DIR, std::make_shared<AdcDir>()},
          {Opcodes::ADC_EXT, std::make_shared<AdcExt>()},
          {Opcodes::ADC_IX2, std::make_shared<AdcIx2>()},
          {Opcodes::ADC_IX1, std::make_shared<AdcIx1>()},
          {Opcodes::ADC_IX, std::make_shared<AdcIx>()},
          {Opcodes::ADC_SP2, std::make_shared<AdcSp2>()},
          {Opcodes::ADC_SP1, std::make_shared<AdcSp1>()},
          {Opcodes::ADD_IMM, std::make_shared<AddImm>()},
          {Opcodes::ADD_DIR, std::make_shared<AddDir>()},
          {Opcodes::ADD_EXT, std::make_shared<AddExt>()},
          {Opcodes::ADD_IX2, std::make_shared<AddIx2>()},
          {Opcodes::ADD_IX1, std::make_shared<AddIx1>()},
          {Opcodes::ADD_IX, std::make_shared<AddIx>()},
          {Opcodes::ADD_SP2, std::make_shared<AddSp2>()},
          {Opcodes::ADD_SP1, std::make_shared<AddSp1>()},
          {Opcodes::AIS_IMM, std::make_shared<AisImm>()},
          {Opcodes::AIX_IMM, std::make_shared<AixImm>()},
          {Opcodes::AND_IMM, std::make_shared<AndImm>()},
          {Opcodes::AND_DIR, std::make_shared<AndDir>()},
          {Opcodes::AND_EXT, std::make_shared<AndExt>()},
          {Opcodes::AND_IX2, std::make_shared<AndIx2>()},
          {Opcodes::AND_IX1, std::make_shared<AndIx1>()},
          {Opcodes::AND_IX, std::make_shared<AndIx>()},
          {Opcodes::AND_SP2, std::make_shared<AndSp2>()},
          {Opcodes::AND_SP1, std::make_shared<AndSp1>()},
          {Opcodes::ASR_DIR, std::make_shared<AsrDir>()},
          {Opcodes::ASRA_INH, std::make_shared<AsraInh>()},
          {Opcodes::ASRX_INH, std::make_shared<AsrxInh>()},
          {Opcodes::ASR_IX1, std::make_shared<AsrIx1>()},
          {Opcodes::ASR_IX, std::make_shared<AsrIx>()},
          {Opcodes::ASR_SP1, std::make_shared<AsrSp1>()},
          {Opcodes::BCC_BHS_REL, std::make_shared<BccBhsRel>()},
          {Opcodes::BCLR_DIR_B0, std::make_shared<BclrDirB0>()},
          {Opcodes::BCLR_DIR_B1, std::make_shared<BclrDirB1>()},
          {Opcodes::BCLR_DIR_B2, std::make_shared<BclrDirB2>()},
          {Opcodes::BCLR_DIR_B3, std::make_shared<BclrDirB3>()},
          {Opcodes::BCLR_DIR_B4, std::make_shared<BclrDirB4>()},
          {Opcodes::BCLR_DIR_B5, std::make_shared<BclrDirB5>()},
          {Opcodes::BCLR_DIR_B6, std::make_shared<BclrDirB6>()},
          {Opcodes::BCLR_DIR_B7, std::make_shared<BclrDirB7>()},
          {Opcodes::BCS_BLO_REL, std::make_shared<BcsBloRel>()},
          {Opcodes::BEQ_REL, std::make_shared<BeqRel>()},
          {Opcodes::BGE_REL, std::make_shared<BgeRel>()},
          {Opcodes::BGND_INH, std::make_shared<BgndInh>()},
          {Opcodes::BGT_REL, std::make_shared<BgtRel>()},
          {Opcodes::BHCC_REL, std::make_shared<BhccRel>()},
          {Opcodes::BHCS_REL, std::make_shared<BhcsRel>()},
          {Opcodes::BHI_REL, std::make_shared<BhiRel>()},
          {Opcodes::BIH_REL, std::make_shared<BihRel>()},
          {Opcodes::BIL_REL, std::make_shared<BilRel>()},
          {Opcodes::BIT_IMM, std::make_shared<BitImm>()},
          {Opcodes::BIT_DIR, std::make_shared<BitDir>()},
          {Opcodes::BIT_EXT, std::make_shared<BitExt>()},
          {Opcodes::BIT_IX2, std::make_shared<BitIx2>()},
          {Opcodes::BIT_IX1, std::make_shared<BitIx1>()},
          {Opcodes::BIT_IX, std::make_shared<BitIx>()},
          {Opcodes::BIT_SP2, std::make_shared<BitSp2>()},
          {Opcodes::BIT_SP1, std::make_shared<BitSp1>()},
          {Opcodes::BLE_REL, std::make_shared<BleRel>()},
          {Opcodes::BLS_REL, std::make_shared<BlsRel>()},
          {Opcodes::BLT_REL, std::make_shared<BltRel>()},
          {Opcodes::BMC_REL, std::make_shared<BmcRel>()},
          {Opcodes::BMI_REL, std::make_shared<BmiRel>()},
          {Opcodes::BMS_REL, std::make_shared<BmsRel>()},
          {Opcodes::BNE_REL, std::make_shared<BneRel>()},
          {Opcodes::BPL_REL, std::make_shared<BplRel>()},
          {Opcodes::BRA_REL, std::make_shared<BraRel>()},
          {Opcodes::BRCLR_DIR_B0, std::make_shared<BrclrDirB0>()},
          {Opcodes::BRCLR_DIR_B1, std::make_shared<BrclrDirB1>()},
          {Opcodes::BRCLR_DIR_B2, std::make_shared<BrclrDirB2>()},
          {Opcodes::BRCLR_DIR_B3, std::make_shared<BrclrDirB3>()},
          {Opcodes::BRCLR_DIR_B4, std::make_shared<BrclrDirB4>()},
          {Opcodes::BRCLR_DIR_B5, std::make_shared<BrclrDirB5>()},
          {Opcodes::BRCLR_DIR_B6, std::make_shared<BrclrDirB6>()},
          {Opcodes::BRCLR_DIR_B7, std::make_shared<BrclrDirB7>()},
          {Opcodes::BRN_REL, std::make_shared<BrnRel>()},
          {Opcodes::BRSET_DIR_B0, std::make_shared<BrsetDirB0>()},
          {Opcodes::BRSET_DIR_B1, std::make_shared<BrsetDirB1>()},
          {Opcodes::BRSET_DIR_B2, std::make_shared<BrsetDirB2>()},
          {Opcodes::BRSET_DIR_B3, std::make_shared<BrsetDirB3>()},
          {Opcodes::BRSET_DIR_B4, std::make_shared<BrsetDirB4>()},
          {Opcodes::BRSET_DIR_B5, std::make_shared<BrsetDirB5>()},
          {Opcodes::BRSET_DIR_B6, std::make_shared<BrsetDirB6>()},
          {Opcodes::BRSET_DIR_B7, std::make_shared<BrsetDirB7>()},
          {Opcodes::BSET_DIR_B0, std::make_shared<BsetDirB0>()},
          {Opcodes::BSET_DIR_B1, std::make_shared<BsetDirB1>()},
          {Opcodes::BSET_DIR_B2, std::make_shared<BsetDirB2>()},
          {Opcodes::BSET_DIR_B3, std::make_shared<BsetDirB3>()},
          {Opcodes::BSET_DIR_B4, std::make_shared<BsetDirB4>()},
          {Opcodes::BSET_DIR_B5, std::make_shared<BsetDirB5>()},
          {Opcodes::BSET_DIR_B6, std::make_shared<BsetDirB6>()},
          {Opcodes::BSET_DIR_B7, std::make_shared<BsetDirB7>()},
          {Opcodes::BSR_REL, std::make_shared<BsrRel>()},
          {Opcodes::CBEQ_DIR, std::make_shared<CbeqDir>()},
          {Opcodes::CBEQA_IMM, std::make_shared<CbeqaImm>()},
          {Opcodes::CBEQX_IMM, std::make_shared<CbeqxImm>()},
          {Opcodes::CBEQ_IX1_POSTINC, std::make_shared<CbeqIx1Postinc>()},
          {Opcodes::CBEQ_IX_POSTINC, std::make_shared<CbeqIxPostinc>()},
          {Opcodes::CBEQ_SP1, std::make_shared<CbeqSp1>()},
          {Opcodes::CLC_INH, std::make_shared<ClcInh>()},
          {Opcodes::CLI_INH, std::make_shared<CliInh>()},
          {Opcodes::CLR_DIR, std::make_shared<ClrDir>()},
          {Opcodes::CLRA_INH, std::make_shared<ClraInh>()},
          {Opcodes::CLRX_INH, std::make_shared<ClrxInh>()},
          {Opcodes::CLRH_INH, std::make_shared<ClrhInh>()},
          {Opcodes::CLR_IX1, std::make_shared<ClrIx1>()},
          {Opcodes::CLR_IX, std::make_shared<ClrIx>()},
          {Opcodes::CLR_SP1, std::make_shared<ClrSp1>()},
          {Opcodes::CMP_IMM, std::make_shared<CmpImm>()},
          {Opcodes::CMP_DIR, std::make_shared<CmpDir>()},
          {Opcodes::CMP_EXT, std::make_shared<CmpExt>()},
          {Opcodes::CMP_IX2, std::make_shared<CmpIx2>()},
          {Opcodes::CMP_IX1, std::make_shared<CmpIx1>()},
          {Opcodes::CMP_IX, std::make_shared<CmpIx>()},
          {Opcodes::CMP_SP2, std::make_shared<CmpSp2>()},
          {Opcodes::CMP_SP1, std::make_shared<CmpSp1>()},
          {Opcodes::COM_DIR, std::make_shared<ComDir>()},
          {Opcodes::COMA_INH, std::make_shared<ComaInh>()},
          {Opcodes::COMX_INH, std::make_shared<ComxInh>()},
          {Opcodes::COM_IX1, std::make_shared<ComIx1>()},
          {Opcodes::COM_IX, std::make_shared<ComIx>()},
          {Opcodes::COM_SP1, std::make_shared<ComSp1>()},
          {Opcodes::CPHX_EXT, std::make_shared<CphxExt>()},
          {Opcodes::CPHX_IMM, std::make_shared<CphxImm>()},
          {Opcodes::CPHX_DIR, std::make_shared<CphxDir>()},
          {Opcodes::CPHX_SP1, std::make_shared<CphxSp1>()},
          {Opcodes::CPX_IMM, std::make_shared<CpxImm>()},
          {Opcodes::CPX_DIR, std::make_shared<CpxDir>()},
          {Opcodes::CPX_EXT, std::make_shared<CpxExt>()},
          {Opcodes::CPX_IX2, std::make_shared<CpxIx2>()},
          {Opcodes::CPX_IX1, std::make_shared<CpxIx1>()},
          {Opcodes::CPX_IX, std::make_shared<CpxIx>()},
          {Opcodes::CPX_SP2, std::make_shared<CpxSp2>()},
          {Opcodes::CPX_SP1, std::make_shared<CpxSp1>()},
          {Opcodes::DAA_INH, std::make_shared<DaaInh>()},
          {Opcodes::DBNZ_DIR, std::make_shared<DbnzDir>()},
          {Opcodes::DBNZA_INH, std::make_shared<DbnzaInh>()},
          {Opcodes::DBNZX_INH, std::make_shared<DbnzxInh>()},
          {Opcodes::DBNZ_IX1, std::make_shared<DbnzIx1>()},
          {Opcodes::DBNZ_IX, std::make_shared<DbnzIx>()},
          {Opcodes::DBNZ_SP1, std::make_shared<DbnzSp1>()},
          {Opcodes::DEC_DIR, std::make_shared<DecDir>()},
          {Opcodes::DECA_INH, std::make_shared<DecaInh>()},
          {Opcodes::DECX_INH, std::make_shared<DecxInh>()},
          {Opcodes::DEC_IX1, std::make_shared<DecIx1>()},
          {Opcodes::DEC_IX, std::make_shared<DecIx>()},
          {Opcodes::DEC_SP1, std::make_shared<DecSp1>()},
          {Opcodes::DIV_INH, std::make_shared<DivInh>()},
          {Opcodes::EOR_IMM, std::make_shared<EorImm>()},
          {Opcodes::EOR_DIR, std::make_shared<EorDir>()},
          {Opcodes::EOR_EXT, std::make_shared<EorExt>()},
          {Opcodes::EOR_IX2, std::make_shared<EorIx2>()},
          {Opcodes::EOR_IX1, std::make_shared<EorIx1>()},
          {Opcodes::EOR_IX, std::make_shared<EorIx>()},
          {Opcodes::EOR_SP2, std::make_shared<EorSp2>()},
          {Opcodes::EOR_SP1, std::make_shared<EorSp1>()},
          {Opcodes::INC_DIR, std::make_shared<IncDir>()},
          {Opcodes::INCA_INH, std::make_shared<IncaInh>()},
          {Opcodes::INCX_INH, std::make_shared<IncxInh>()},
          {Opcodes::INC_IX1, std::make_shared<IncIx1>()},
          {Opcodes::INC_IX, std::make_shared<IncIx>()},
          {Opcodes::INC_SP1, std::make_shared<IncSp1>()},
          {Opcodes::JMP_DIR, std::make_shared<JmpDir>()},
          {Opcodes::JMP_EXT, std::make_shared<JmpExt>()},
          {Opcodes::JMP_IX2, std::make_shared<JmpIx2>()},
          {Opcodes::JMP_IX1, std::make_shared<JmpIx1>()},
          {Opcodes::JMP_IX, std::make_shared<JmpIx>()},
          {Opcodes::JSR_DIR, std::make_shared<JsrDir>()},
          {Opcodes::JSR_EXT, std::make_shared<JsrExt>()},
          {Opcodes::JSR_IX2, std::make_shared<JsrIx2>()},
          {Opcodes::JSR_IX1, std::make_shared<JsrIx1>()},
          {Opcodes::JSR_IX, std::make_shared<JsrIx>()},
          {Opcodes::LDA_IMM, std::make_shared<LdaImm>()},
          {Opcodes::LDA_DIR, std::make_shared<LdaDir>()},
          {Opcodes::LDA_EXT, std::make_shared<LdaExt>()},
          {Opcodes::LDA_IX2, std::make_shared<LdaIx2>()},
          {Opcodes::LDA_IX1, std::make_shared<LdaIx1>()},
          {Opcodes::LDA_IX, std::make_shared<LdaIx>()},
          {Opcodes::LDA_SP2, std::make_shared<LdaSp2>()},
          {Opcodes::LDA_SP1, std::make_shared<LdaSp1>()},
          {Opcodes::LDHX_IMM, std::make_shared<LdhxImm>()},
          {Opcodes::LDHX_DIR, std::make_shared<LdhxDir>()},
          {Opcodes::LDHX_EXT, std::make_shared<LdhxExt>()},
          {Opcodes::LDHX_IX, std::make_shared<LdhxIx>()},
          {Opcodes::LDHX_IX2, std::make_shared<LdhxIx2>()},
          {Opcodes::LDHX_IX1, std::make_shared<LdhxIx1>()},
          {Opcodes::LDHX_SP1, std::make_shared<LdhxSp1>()},
          {Opcodes::LDX_IMM, std::make_shared<LdxImm>()},
          {Opcodes::LDX_DIR, std::make_shared<LdxDir>()},
          {Opcodes::LDX_EXT, std::make_shared<LdxExt>()},
          {Opcodes::LDX_IX2, std::make_shared<LdxIx2>()},
          {Opcodes::LDX_IX1, std::make_shared<LdxIx1>()},
          {Opcodes::LDX_IX, std::make_shared<LdxIx>()},
          {Opcodes::LDX_SP2, std::make_shared<LdxSp2>()},
          {Opcodes::LDX_SP1, std::make_shared<LdxSp1>()},
          {Opcodes::LSL_DIR, std::make_shared<LslDir>()},
          {Opcodes::LSLA_INH, std::make_shared<LslaInh>()},
          {Opcodes::LSLX_INH, std::make_shared<LslxInh>()},
          {Opcodes::LSL_IX1, std::make_shared<LslIx1>()},
          {Opcodes::LSL_IX, std::make_shared<LslIx>()},
          {Opcodes::LSL_SP1, std::make_shared<LslSp1>()},
          {Opcodes::LSR_DIR, std::make_shared<LsrDir>()},
          {Opcodes::LSRA_INH, std::make_shared<LsraInh>()},
          {Opcodes::LSRX_INH, std::make_shared<LsrxInh>()},
          {Opcodes::LSR_IX1, std::make_shared<LsrIx1>()},
          {Opcodes::LSR_IX, std::make_shared<LsrIx>()},
          {Opcodes::LSR_SP1, std::make_shared<LsrSp1>()},
          {Opcodes::MOV_DIR_DIR, std::make_shared<MovDirDir>()},
          {Opcodes::MOV_DIR_IX_POSTINC, std::make_shared<MovDirIxPostinc>()},
          {Opcodes::MOV_IMM_DIR, std::make_shared<MovImmDir>()},
          {Opcodes::MOV_IX_POSTINC_DIR, std::make_shared<MovIxPostincDir>()},
          {Opcodes::MUL_INH, std::make_shared<MulInh>()},
          {Opcodes::NEG_DIR, std::make_shared<NegDir>()},
          {Opcodes::NEGA_INH, std::make_shared<NegaInh>()},
          {Opcodes::NEGX_INH, std::make_shared<NegxInh>()},
          {Opcodes::NEG_IX1, std::make_shared<NegIx1>()},
          {Opcodes::NEG_IX, std::make_shared<NegIx>()},
          {Opcodes::NEG_SP1, std::make_shared<NegSp1>()},
          {Opcodes::NOP_INH, std::make_shared<NopInh>()},
          {Opcodes::NSA_INH, std::make_shared<NsaInh>()},
          {Opcodes::ORA_IMM, std::make_shared<OraImm>()},
          {Opcodes::ORA_DIR, std::make_shared<OraDir>()},
          {Opcodes::ORA_EXT, std::make_shared<OraExt>()},
          {Opcodes::ORA_IX2, std::make_shared<OraIx2>()},
          {Opcodes::ORA_IX1, std::make_shared<OraIx1>()},
          {Opcodes::ORA_IX, std::make_shared<OraIx>()},
          {Opcodes::ORA_SP2, std::make_shared<OraSp2>()},
          {Opcodes::ORA_SP1, std::make_shared<OraSp1>()},
          {Opcodes::PSHA_INH, std::make_shared<PshaInh>()},
          {Opcodes::PSHH_INH, std::make_shared<PshhInh>()},
          {Opcodes::PSHX_INH, std::make_shared<PshxInh>()},
          {Opcodes::PULA_INH, std::make_shared<PulaInh>()},
          {Opcodes::PULH_INH, std::make_shared<PulhInh>()},
          {Opcodes::PULX_INH, std::make_shared<PulxInh>()},
          {Opcodes::ROL_DIR, std::make_shared<RolDir>()},
          {Opcodes::ROLA_INH, std::make_shared<RolaInh>()},
          {Opcodes::ROLX_INH, std::make_shared<RolxInh>()},
          {Opcodes::ROL_IX1, std::make_shared<RolIx1>()},
          {Opcodes::ROL_IX, std::make_shared<RolIx>()},
          {Opcodes::ROL_SP1, std::make_shared<RolSp1>()},
          {Opcodes::ROR_DIR, std::make_shared<RorDir>()},
          {Opcodes::RORA_INH, std::make_shared<RoraInh>()},
          {Opcodes::RORX_INH, std::make_shared<RorxInh>()},
          {Opcodes::ROR_IX1, std::make_shared<RorIx1>()},
          {Opcodes::ROR_IX, std::make_shared<RorIx>()},
          {Opcodes::ROR_SP1, std::make_shared<RorSp1>()},
          {Opcodes::RSP_INH, std::make_shared<RspInh>()},
          {Opcodes::RTI_INH, std::make_shared<RtiInh>()},
          {Opcodes::RTS_INH, std::make_shared<RtsInh>()},
          {Opcodes::SBC_IMM, std::make_shared<SbcImm>()},
          {Opcodes::SBC_DIR, std::make_shared<SbcDir>()},
          {Opcodes::SBC_EXT, std::make_shared<SbcExt>()},
          {Opcodes::SBC_IX2, std::make_shared<SbcIx2>()},
          {Opcodes::SBC_IX1, std::make_shared<SbcIx1>()},
          {Opcodes::SBC_IX, std::make_shared<SbcIx>()},
          {Opcodes::SBC_SP2, std::make_shared<SbcSp2>()},
          {Opcodes::SBC_SP1, std::make_shared<SbcSp1>()},
          {Opcodes::SEC_INH, std::make_shared<SecInh>()},
          {Opcodes::SEI_INH, std::make_shared<SeiInh>()},
          {Opcodes::STA_DIR, std::make_shared<StaDir>()},
          {Opcodes::STA_EXT, std::make_shared<StaExt>()},
          {Opcodes::STA_IX2, std::make_shared<StaIx2>()},
          {Opcodes::STA_IX1, std::make_shared<StaIx1>()},
          {Opcodes::STA_IX, std::make_shared<StaIx>()},
          {Opcodes::STA_SP2, std::make_shared<StaSp2>()},
          {Opcodes::STA_SP1, std::make_shared<StaSp1>()},
          {Opcodes::STHX_DIR, std::make_shared<SthxDir>()},
          {Opcodes::STHX_EXT, std::make_shared<SthxExt>()},
          {Opcodes::STHX_SP1, std::make_shared<SthxSp1>()},
          {Opcodes::STOP_INH, std::make_shared<StopInh>()},
          {Opcodes::STX_DIR, std::make_shared<StxDir>()},
          {Opcodes::STX_EXT, std::make_shared<StxExt>()},
          {Opcodes::STX_IX2, std::make_shared<StxIx2>()},
          {Opcodes::STX_IX1, std::make_shared<StxIx1>()},
          {Opcodes::STX_IX, std::make_shared<StxIx>()},
          {Opcodes::STX_SP2, std::make_shared<StxSp2>()},
          {Opcodes::STX_SP1, std::make_shared<StxSp1>()},
          {Opcodes::SUB_IMM, std::make_shared<SubImm>()},
          {Opcodes::SUB_DIR, std::make_shared<SubDir>()},
          {Opcodes::SUB_EXT, std::make_shared<SubExt>()},
          {Opcodes::SUB_IX2, std::make_shared<SubIx2>()},
          {Opcodes::SUB_IX1, std::make_shared<SubIx1>()},
          {Opcodes::SUB_IX, std::make_shared<SubIx>()},
          {Opcodes::SUB_SP2, std::make_shared<SubSp2>()},
          {Opcodes::SUB_SP1, std::make_shared<SubSp1>()},
          {Opcodes::SWI_INH, std::make_shared<SwiInh>()},
          {Opcodes::TAP_INH, std::make_shared<TapInh>()},
          {Opcodes::TAX_INH, std::make_shared<TaxInh>()},
          {Opcodes::TPA_INH, std::make_shared<TpaInh>()},
          {Opcodes::TST_DIR, std::make_shared<TstDir>()},
          {Opcodes::TSTA_INH, std::make_shared<TstaInh>()},
          {Opcodes::TSTX_INH, std::make_shared<TstxInh>()},
          {Opcodes::TST_IX1, std::make_shared<TstIx1>()},
          {Opcodes::TST_IX, std::make_shared<TstIx>()},
          {Opcodes::TST_SP1, std::make_shared<TstSp1>()},
          {Opcodes::TSX_INH, std::make_shared<TsxInh>()},
          {Opcodes::TXA_INH, std::make_shared<TxaInh>()},
          {Opcodes::TXS_INH, std::make_shared<TxsInh>()},
          {Opcodes::WAIT_INH, std::make_shared<WaitInh>()},
      };
  return map;
}

}  // namespace HCS08
