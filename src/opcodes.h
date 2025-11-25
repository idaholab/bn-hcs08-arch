// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef HCS08_OPCODES_H
#define HCS08_OPCODES_H

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace HCS08::Opcodes {
std::vector<int> GetAllOpcodes();
std::vector<int> GetNonBranchingOpcodes();

// ADC - Add with carry
constexpr uint32_t ADC_IMM = 0xA9;    // ADC #opr8i
constexpr uint32_t ADC_DIR = 0xB9;    // ADC opr8a
constexpr uint32_t ADC_EXT = 0xC9;    // ADC opr16a
constexpr uint32_t ADC_IX2 = 0xD9;    // ADC oprx16,X
constexpr uint32_t ADC_IX1 = 0xE9;    // ADC oprx8,X
constexpr uint32_t ADC_IX = 0xF9;     // ADC ,X
constexpr uint32_t ADC_SP2 = 0x9ED9;  // ADC oprx16,SP
constexpr uint32_t ADC_SP1 = 0x9EE9;  // ADC oprx8,SP

// ADD - Add without carry
constexpr uint32_t ADD_IMM = 0xAB;    // ADD #opr8i
constexpr uint32_t ADD_DIR = 0xBB;    // ADD opr8a
constexpr uint32_t ADD_EXT = 0xCB;    // ADD opr16a
constexpr uint32_t ADD_IX2 = 0xDB;    // ADD oprx16,X
constexpr uint32_t ADD_IX1 = 0xEB;    // ADD oprx8,X
constexpr uint32_t ADD_IX = 0xFB;     // ADD ,X
constexpr uint32_t ADD_SP2 = 0x9EDB;  // ADD oprx16,SP
constexpr uint32_t ADD_SP1 = 0x9EEB;  // ADD oprx8,SP

// AIS - Add immediate value (signed) to stack pointer
constexpr uint32_t AIS_IMM = 0xA7;  // AIS #opr8i

// AIX - Add immediate value (signed) to index register
constexpr uint32_t AIX_IMM = 0xAF;  // AIX #opr8i

// AND - Logical AND
constexpr uint32_t AND_IMM = 0xA4;    // AND #opr8i
constexpr uint32_t AND_DIR = 0xB4;    // AND opr8a
constexpr uint32_t AND_EXT = 0xC4;    // AND opr16a
constexpr uint32_t AND_IX2 = 0xD4;    // AND oprx16,X
constexpr uint32_t AND_IX1 = 0xE4;    // AND oprx8,X
constexpr uint32_t AND_IX = 0xF4;     // AND ,X
constexpr uint32_t AND_SP2 = 0x9ED4;  // AND oprx16,SP
constexpr uint32_t AND_SP1 = 0x9EE4;  // AND oprx8,SP

// ASL - Arithmetic shift left (exact same as LSL - both are just a shift left)
// constexpr uint32_t ASL_DIR  = 0x38;  // ASL opr8a
// constexpr uint32_t ASLA_INH = 0x48;  // ASLA
// constexpr uint32_t ASLX_INH = 0x58;  // ASLX
// constexpr uint32_t ASL_IX1  = 0x68;  // ASL oprx8,X
// constexpr uint32_t ASL_IX   = 0x78;  // ASL ,X
// constexpr uint32_t ASL_SP1  = 0x9E68 // ASL, oprx8,SP

// ASR - Arithmetic shift right
constexpr uint32_t ASR_DIR = 0x37;    // ASR opr8a
constexpr uint32_t ASRA_INH = 0x47;   // ASRA
constexpr uint32_t ASRX_INH = 0x57;   // ASRX
constexpr uint32_t ASR_IX1 = 0x67;    // ASR oprx8,X
constexpr uint32_t ASR_IX = 0x77;     // ASR ,X
constexpr uint32_t ASR_SP1 = 0x9E67;  // ASR, oprx8,SP

// BCC - Branch if carry bit clear; BHS - Branch if higher or same (same opcode
// and behavior)
constexpr uint32_t BCC_BHS_REL = 0x24;  // BCC rel; BHS rel

// BCLR n - Clear bit n in memory
constexpr uint32_t BCLR_DIR_B0 = 0x11;  // BCLR 0,opr8a
constexpr uint32_t BCLR_DIR_B1 = 0x13;  // BCLR 1,opr8a
constexpr uint32_t BCLR_DIR_B2 = 0x15;  // BCLR 2,opr8a
constexpr uint32_t BCLR_DIR_B3 = 0x17;  // BCLR 3,opr8a
constexpr uint32_t BCLR_DIR_B4 = 0x19;  // BCLR 4,opr8a
constexpr uint32_t BCLR_DIR_B5 = 0x1B;  // BCLR 5,opr8a
constexpr uint32_t BCLR_DIR_B6 = 0x1D;  // BCLR 6,opr8a
constexpr uint32_t BCLR_DIR_B7 = 0x1F;  // BCLR 7,opr8a

// BCS - Branch if carry bit set; BLO - Branch if lower (same opcode and
// behavior)
constexpr uint32_t BCS_BLO_REL = 0x25;  // BCS rel; BLO rel

// BEQ - Branch if equal
constexpr uint32_t BEQ_REL = 0x27;  // BEQ rel

// BGE - Branch if greater than or equal to
constexpr uint32_t BGE_REL = 0x90;  // BGE rel

// BGND - background
constexpr uint32_t BGND_INH = 0x82;  // BGND

// BGT - Branch if greater than
constexpr uint32_t BGT_REL = 0x92;  // BGT rel

// BHCC - Branch if half carry bit clear
constexpr uint32_t BHCC_REL = 0x28;  // BHCC rel

// BHCS - Branch if half carry bit set
constexpr uint32_t BHCS_REL = 0x29;  // BHCS rel

// BHI - Branch if higher
constexpr uint32_t BHI_REL = 0x22;  // BHI rel

// BIH - Branch if IRQ pin high
constexpr uint32_t BIH_REL = 0x2F;  // BIH rel

// BIL - Branch if IRQ pin low
constexpr uint32_t BIL_REL = 0x2E;  // BIL rel

// BIT - Bit test
constexpr uint32_t BIT_IMM = 0xA5;    // BIT #opr8i
constexpr uint32_t BIT_DIR = 0xB5;    // BIT opr8a
constexpr uint32_t BIT_EXT = 0xC5;    // BIT opr16a
constexpr uint32_t BIT_IX2 = 0xD5;    // BIT oprx16,X
constexpr uint32_t BIT_IX1 = 0xE5;    // BIT oprx8,X
constexpr uint32_t BIT_IX = 0xF5;     // BIT ,X
constexpr uint32_t BIT_SP2 = 0x9ED5;  // BIT oprx16,SP
constexpr uint32_t BIT_SP1 = 0x9EE5;  // BIT oprx8,SP

// BLE - Branch if less than or equal to
constexpr uint32_t BLE_REL = 0x93;  // BLE rel

// BLS - Branch if lower or same
constexpr uint32_t BLS_REL = 0x23;  // BLS rel

// BLT - Branch if less than (signed operands)
constexpr uint32_t BLT_REL = 0x91;  // BLT rel

// BMC - Branch if interrupt mask clear
constexpr uint32_t BMC_REL = 0x2C;  // BMC rel

// BMI - Branch if minus
constexpr uint32_t BMI_REL = 0x2B;  // BMI rel

// BMS - Branch if interrupt mask set
constexpr uint32_t BMS_REL = 0x2D;  // BMS rel

// BNE - Branch if not equal
constexpr uint32_t BNE_REL = 0x26;  // BNE rel

// BPL - Branch if plus
constexpr uint32_t BPL_REL = 0x2A;  // BPL rel

// BRA - Branch always
constexpr uint32_t BRA_REL = 0x20;  // BRA rel

// BRCLR n - Branch if bit n in memory clear
constexpr uint32_t BRCLR_DIR_B0 = 0x01;  // BRCLR 0,opr8a,rel
constexpr uint32_t BRCLR_DIR_B1 = 0x03;  // BRCLR 1,opr8a,rel
constexpr uint32_t BRCLR_DIR_B2 = 0x05;  // BRCLR 2,opr8a,rel
constexpr uint32_t BRCLR_DIR_B3 = 0x07;  // BRCLR 3,opr8a,rel
constexpr uint32_t BRCLR_DIR_B4 = 0x09;  // BRCLR 4,opr8a,rel
constexpr uint32_t BRCLR_DIR_B5 = 0x0B;  // BRCLR 5,opr8a,rel
constexpr uint32_t BRCLR_DIR_B6 = 0x0D;  // BRCLR 6,opr8a,rel
constexpr uint32_t BRCLR_DIR_B7 = 0x0F;  // BRCLR 7,opr8a,rel

// BRN - Branch never; basically a 2-byte NOP
constexpr uint32_t BRN_REL = 0x21;  // BRN rel

// BRSET n - Branch if bit n in memory set
constexpr uint32_t BRSET_DIR_B0 = 0x00;  // BRSET 0,opr8a,rel
constexpr uint32_t BRSET_DIR_B1 = 0x02;  // BRSET 1,opr8a,rel
constexpr uint32_t BRSET_DIR_B2 = 0x04;  // BRSET 2,opr8a,rel
constexpr uint32_t BRSET_DIR_B3 = 0x06;  // BRSET 3,opr8a,rel
constexpr uint32_t BRSET_DIR_B4 = 0x08;  // BRSET 4,opr8a,rel
constexpr uint32_t BRSET_DIR_B5 = 0x0A;  // BRSET 5,opr8a,rel
constexpr uint32_t BRSET_DIR_B6 = 0x0C;  // BRSET 6,opr8a,rel
constexpr uint32_t BRSET_DIR_B7 = 0x0E;  // BRSET 7,opr8a,rel

// BSET n - Set bit n in memory
constexpr uint32_t BSET_DIR_B0 = 0x10;  // BSET 0,opr8a
constexpr uint32_t BSET_DIR_B1 = 0x12;  // BSET 1,opr8a
constexpr uint32_t BSET_DIR_B2 = 0x14;  // BSET 2,opr8a
constexpr uint32_t BSET_DIR_B3 = 0x16;  // BSET 3,opr8a
constexpr uint32_t BSET_DIR_B4 = 0x18;  // BSET 4,opr8a
constexpr uint32_t BSET_DIR_B5 = 0x1A;  // BSET 5,opr8a
constexpr uint32_t BSET_DIR_B6 = 0x1C;  // BSET 6,opr8a
constexpr uint32_t BSET_DIR_B7 = 0x1E;  // BSET 7,opr8a

// BSR - Branch to subroutine
constexpr uint32_t BSR_REL = 0xAD;  // BSR rel

// CBEQ - Compare and branch if equal
constexpr uint32_t CBEQ_DIR = 0x31;          // CBEQ opr8a,rel
constexpr uint32_t CBEQA_IMM = 0x41;         // CBEQA #opr8i,rel
constexpr uint32_t CBEQX_IMM = 0x51;         // CBEQX #opr8i,rel
constexpr uint32_t CBEQ_IX1_POSTINC = 0x61;  // CBEQ oprx8,X+,rel
constexpr uint32_t CBEQ_IX_POSTINC = 0x71;   // CBEQ ,X+,rel
constexpr uint32_t CBEQ_SP1 = 0x9E61;        // CBEQ oprx8,SP,rel

// CLC - Clear carry bit
constexpr uint32_t CLC_INH = 0x98;  // CLC

// CLI - Clear interrupt mask bit
constexpr uint32_t CLI_INH = 0x9A;  // CLI

// CLR - Clear
constexpr uint32_t CLR_DIR = 0x3F;    // CLR opr8a
constexpr uint32_t CLRA_INH = 0x4F;   // CLRA
constexpr uint32_t CLRX_INH = 0x5F;   // CLRX
constexpr uint32_t CLRH_INH = 0x8C;   // CLRH
constexpr uint32_t CLR_IX1 = 0x6F;    // CLR oprx8,X
constexpr uint32_t CLR_IX = 0x7F;     // CLR ,X
constexpr uint32_t CLR_SP1 = 0x9E6F;  // CLR oprx8,SP

// CMP - Compare accumulator with memory
constexpr uint32_t CMP_IMM = 0xA1;    // CMP #opr8i
constexpr uint32_t CMP_DIR = 0xB1;    // CMP opr8a
constexpr uint32_t CMP_EXT = 0xC1;    // CMP opr16a
constexpr uint32_t CMP_IX2 = 0xD1;    // CMP oprx16,X
constexpr uint32_t CMP_IX1 = 0xE1;    // CMP oprx8,X
constexpr uint32_t CMP_IX = 0xF1;     // CMP ,X
constexpr uint32_t CMP_SP2 = 0x9ED1;  // CMP oprx16,SP
constexpr uint32_t CMP_SP1 = 0x9EE1;  // CMP oprx8,SP

// COM - Complement (one's complement)
constexpr uint32_t COM_DIR = 0x33;    // COM opr8a
constexpr uint32_t COMA_INH = 0x43;   // COMA
constexpr uint32_t COMX_INH = 0x53;   // COMX
constexpr uint32_t COM_IX1 = 0x63;    // COM oprx8,X
constexpr uint32_t COM_IX = 0x73;     // COM ,X
constexpr uint32_t COM_SP1 = 0x9E63;  // COM, oprx8,SP

// CPHX - Compare index register with memory
constexpr uint32_t CPHX_EXT = 0x3E;    // CPHX opr16a
constexpr uint32_t CPHX_IMM = 0x65;    // CPHX #opr16i
constexpr uint32_t CPHX_DIR = 0x75;    // CPHX opr8a
constexpr uint32_t CPHX_SP1 = 0x9EF3;  // CPHX oprx8,SP

// CPX - Compare X (index register low) with memory
constexpr uint32_t CPX_IMM = 0xA3;    // CPX #opr8i
constexpr uint32_t CPX_DIR = 0xB3;    // CPX opr8a
constexpr uint32_t CPX_EXT = 0xC3;    // CPX opr16a
constexpr uint32_t CPX_IX2 = 0xD3;    // CPX oprx16,X
constexpr uint32_t CPX_IX1 = 0xE3;    // CPX oprx8,X
constexpr uint32_t CPX_IX = 0xF3;     // CPX ,X
constexpr uint32_t CPX_SP2 = 0x9ED3;  // CPX oprx16,SP
constexpr uint32_t CPX_SP1 = 0x9EE3;  // CPX oprx8,SP

// DAA - Decimal adjust accumulator
constexpr uint32_t DAA_INH = 0x72;  // DAA

// DBNZ - Decrement and branch if not zero
constexpr uint32_t DBNZ_DIR = 0x3B;    // DBNZ opr8a,rel
constexpr uint32_t DBNZA_INH = 0x4B;   // DBNZA rel
constexpr uint32_t DBNZX_INH = 0x5B;   // DBNZX rel
constexpr uint32_t DBNZ_IX1 = 0x6B;    // DBNZ oprx8,X,rel
constexpr uint32_t DBNZ_IX = 0x7B;     // DBNZ ,X,rel
constexpr uint32_t DBNZ_SP1 = 0x9E6B;  // DBNZ oprx8,SP,rel

// DEC - Decrement
constexpr uint32_t DEC_DIR = 0x3A;    // DEC opr8a
constexpr uint32_t DECA_INH = 0x4A;   // DECA
constexpr uint32_t DECX_INH = 0x5A;   // DECX
constexpr uint32_t DEC_IX1 = 0x6A;    // DEC oprx8,X
constexpr uint32_t DEC_IX = 0x7A;     // DEC ,X
constexpr uint32_t DEC_SP1 = 0x9E6A;  // DEC oprx8,SP

// DIV - Divide
constexpr uint32_t DIV_INH = 0x52;  // DIV

// EOR = Exclusive-OR memory with accumulator
constexpr uint32_t EOR_IMM = 0xA8;    // EOR #opr8i
constexpr uint32_t EOR_DIR = 0xB8;    // EOR opr8a
constexpr uint32_t EOR_EXT = 0xC8;    // EOR opr16a
constexpr uint32_t EOR_IX2 = 0xD8;    // EOR oprx16,X
constexpr uint32_t EOR_IX1 = 0xE8;    // EOR oprx8,X
constexpr uint32_t EOR_IX = 0xF8;     // EOR ,X
constexpr uint32_t EOR_SP2 = 0x9ED8;  // EOR oprx16,SP
constexpr uint32_t EOR_SP1 = 0x9EE8;  // EOR oprx8,SP

// INC - Increment
constexpr uint32_t INC_DIR = 0x3C;    // INC opr8a
constexpr uint32_t INCA_INH = 0x4C;   // INCA
constexpr uint32_t INCX_INH = 0x5C;   // INCX
constexpr uint32_t INC_IX1 = 0x6C;    // INC oprx8,X
constexpr uint32_t INC_IX = 0x7C;     // INC ,X
constexpr uint32_t INC_SP1 = 0x9E6C;  // INC oprx8,SP

// JMP - Jump
constexpr uint32_t JMP_DIR = 0xBC;  // JMP opr8a
constexpr uint32_t JMP_EXT = 0xCC;  // JMP opr16a
constexpr uint32_t JMP_IX2 = 0xDC;  // JMP oprx16,X
constexpr uint32_t JMP_IX1 = 0xEC;  // JMP oprx8,X
constexpr uint32_t JMP_IX = 0xFC;   // JMP ,X

// JSR - Jump to subroutine
constexpr uint32_t JSR_DIR = 0xBD;  // JSR opr8a
constexpr uint32_t JSR_EXT = 0xCD;  // JSR opr16a
constexpr uint32_t JSR_IX2 = 0xDD;  // JSR oprx16,X
constexpr uint32_t JSR_IX1 = 0xED;  // JSR oprx8,X
constexpr uint32_t JSR_IX = 0xFD;   // JSR ,X

// LDA - Load accumulator from memory
constexpr uint32_t LDA_IMM = 0xA6;    // LDA #opr8i
constexpr uint32_t LDA_DIR = 0xB6;    // LDA opr8a
constexpr uint32_t LDA_EXT = 0xC6;    // LDA opr16a
constexpr uint32_t LDA_IX2 = 0xD6;    // LDA oprx16,X
constexpr uint32_t LDA_IX1 = 0xE6;    // LDA oprx8,X
constexpr uint32_t LDA_IX = 0xF6;     // LDA ,X
constexpr uint32_t LDA_SP2 = 0x9ED6;  // LDA oprx16,SP
constexpr uint32_t LDA_SP1 = 0x9EE6;  // LDA oprx8,SP

// LDHX - Load index register from memory
constexpr uint32_t LDHX_IMM = 0x45;    // LDHX #opr16i
constexpr uint32_t LDHX_DIR = 0x55;    // LDHX opr8a
constexpr uint32_t LDHX_EXT = 0x32;    // LDHX opr16a
constexpr uint32_t LDHX_IX = 0x9EAE;   // LDHX ,X
constexpr uint32_t LDHX_IX2 = 0x9EBE;  // LDHX oprx16,X
constexpr uint32_t LDHX_IX1 = 0x9ECE;  // LDHX oprx8,X
constexpr uint32_t LDHX_SP1 = 0x9EFE;  // LDHX oprx8,SP

// LDX - Load X (index register low) from memory
constexpr uint32_t LDX_IMM = 0xAE;    // LDX #opr8i
constexpr uint32_t LDX_DIR = 0xBE;    // LDX opr8a
constexpr uint32_t LDX_EXT = 0xCE;    // LDX opr16a
constexpr uint32_t LDX_IX2 = 0xDE;    // LDX oprx16,X
constexpr uint32_t LDX_IX1 = 0xEE;    // LDX oprx8,X
constexpr uint32_t LDX_IX = 0xFE;     // LDX ,X
constexpr uint32_t LDX_SP2 = 0x9EDE;  // LDX oprx16,SP
constexpr uint32_t LDX_SP1 = 0x9EEE;  // LDX oprx8,SP

// LSL - Logical shift left (same as ASL)
constexpr uint32_t LSL_DIR = 0x38;    // LSL opr8a
constexpr uint32_t LSLA_INH = 0x48;   // LSLA
constexpr uint32_t LSLX_INH = 0x58;   // LSLX
constexpr uint32_t LSL_IX1 = 0x68;    // LSL oprx8,X
constexpr uint32_t LSL_IX = 0x78;     // LSL ,X
constexpr uint32_t LSL_SP1 = 0x9E68;  // LSL, oprx8,SP

// LSR - Logical shift right
constexpr uint32_t LSR_DIR = 0x34;    // LSR opr8a
constexpr uint32_t LSRA_INH = 0x44;   // LSRA
constexpr uint32_t LSRX_INH = 0x54;   // LSRX
constexpr uint32_t LSR_IX1 = 0x64;    // LSR oprx8,X
constexpr uint32_t LSR_IX = 0x74;     // LSR ,X
constexpr uint32_t LSR_SP1 = 0x9E64;  // LSR, oprx8,SP

// MOV - Move
constexpr uint32_t MOV_DIR_DIR = 0x4E;         // MOV opr8a,opr8a
constexpr uint32_t MOV_DIR_IX_POSTINC = 0x5E;  // MOV opr8a,X+
constexpr uint32_t MOV_IMM_DIR = 0x6E;         // MOV #opr8i,opr8a
constexpr uint32_t MOV_IX_POSTINC_DIR = 0x7E;  // MOV ,X+,opr8a

// MUL - Unsigned multiply
constexpr uint32_t MUL_INH = 0x42;  // MUL

// NEG - Negate (two's complement)
constexpr uint32_t NEG_DIR = 0x30;    // NEG opr8a
constexpr uint32_t NEGA_INH = 0x40;   // NEGA
constexpr uint32_t NEGX_INH = 0x50;   // NEGX
constexpr uint32_t NEG_IX1 = 0x60;    // NEG oprx8,X
constexpr uint32_t NEG_IX = 0x70;     // NEG ,X
constexpr uint32_t NEG_SP1 = 0x9E60;  // NEG oprx8,SP

// NOP - No operation
constexpr uint32_t NOP_INH = 0x9D;  // NOP

// NSA - Nibble swap accumulator
constexpr uint32_t NSA_INH = 0x62;  // NSA

// ORA - Inclusive-OR accumulator and memory
constexpr uint32_t ORA_IMM = 0xAA;    // ORA #opr8i
constexpr uint32_t ORA_DIR = 0xBA;    // ORA opr8a
constexpr uint32_t ORA_EXT = 0xCA;    // ORA opr16a
constexpr uint32_t ORA_IX2 = 0xDA;    // ORA oprx16,X
constexpr uint32_t ORA_IX1 = 0xEA;    // ORA oprx8,X
constexpr uint32_t ORA_IX = 0xFA;     // ORA ,X
constexpr uint32_t ORA_SP2 = 0x9EDA;  // ORA oprx16,SP
constexpr uint32_t ORA_SP1 = 0x9EEA;  // ORA oprx8,SP

// PSHA - Push accumulator onto stack
constexpr uint32_t PSHA_INH = 0x87;  // PSHA

// PSHH - Push H (index register high) onto stack
constexpr uint32_t PSHH_INH = 0x8B;  // PSHH

// PSHX - Push X (index register low) onto stack
constexpr uint32_t PSHX_INH = 0x89;  // INH

// PULA - Pull accumulator from stack
constexpr uint32_t PULA_INH = 0x86;  // PULA

// PULH - Pull H (index register high) from stack
constexpr uint32_t PULH_INH = 0x8A;  // PULH

// PULX - Pull X (index register low) from stack
constexpr uint32_t PULX_INH = 0x88;  // PULX

// ROL - Rotate left through carry
constexpr uint32_t ROL_DIR = 0x39;    // ROL opr8a
constexpr uint32_t ROLA_INH = 0x49;   // ROLA
constexpr uint32_t ROLX_INH = 0x59;   // ROLX
constexpr uint32_t ROL_IX1 = 0x69;    // ROL oprx8,X
constexpr uint32_t ROL_IX = 0x79;     // ROL ,X
constexpr uint32_t ROL_SP1 = 0x9E69;  // ROL oprx8,SP

// ROR - Rotate right through carry
constexpr uint32_t ROR_DIR = 0x36;    // ROR opr8a
constexpr uint32_t RORA_INH = 0x46;   // RORA
constexpr uint32_t RORX_INH = 0x56;   // RORX
constexpr uint32_t ROR_IX1 = 0x66;    // ROR oprx8,X
constexpr uint32_t ROR_IX = 0x76;     // ROR ,X
constexpr uint32_t ROR_SP1 = 0x9E66;  // ROR oprx8,SP

// RSP - Reset stack pointer
constexpr uint32_t RSP_INH = 0x9C;  // RSP

// RTI - Return from interrupt
constexpr uint32_t RTI_INH = 0x80;  // RTI

// RTS - Return from subroutine
constexpr uint32_t RTS_INH = 0x81;  // RTS

// SBC - Subtract with carry
constexpr uint32_t SBC_IMM = 0xA2;    // SBC #opr8i
constexpr uint32_t SBC_DIR = 0xB2;    // SBC opr8a
constexpr uint32_t SBC_EXT = 0xC2;    // SBC opr16a
constexpr uint32_t SBC_IX2 = 0xD2;    // SBC oprx16,X
constexpr uint32_t SBC_IX1 = 0xE2;    // SBC oprx8,X
constexpr uint32_t SBC_IX = 0xF2;     // SBC ,X
constexpr uint32_t SBC_SP2 = 0x9ED2;  // SBC oprx16,SP
constexpr uint32_t SBC_SP1 = 0x9EE2;  // SBC oprx8,SP

// SEC - Set carry bit
constexpr uint32_t SEC_INH = 0x99;  // SEC

// SEI - Set interrupt mask bit
constexpr uint32_t SEI_INH = 0x9B;  // SEI

// STA - Store accumulator in memory
constexpr uint32_t STA_DIR = 0xB7;    // STA opr8a
constexpr uint32_t STA_EXT = 0xC7;    // STA opr16a
constexpr uint32_t STA_IX2 = 0xD7;    // STA oprx16,X
constexpr uint32_t STA_IX1 = 0xE7;    // STA oprx8,X
constexpr uint32_t STA_IX = 0xF7;     // STA ,X
constexpr uint32_t STA_SP2 = 0x9ED7;  // STA oprx16,SP
constexpr uint32_t STA_SP1 = 0x9EE7;  // STA oprx8,SP

// STHX - Store index register
constexpr uint32_t STHX_DIR = 0x35;    // STHX opr8a
constexpr uint32_t STHX_EXT = 0x96;    // STHX opr16a
constexpr uint32_t STHX_SP1 = 0x9EFF;  // STHX oprx8,SP

// STOP - Enable IRQ pin, stop processing
constexpr uint32_t STOP_INH = 0x8E;  // STOP

// STX - Store X (index register low) in memory
constexpr uint32_t STX_DIR = 0xBF;    // STX opr8a
constexpr uint32_t STX_EXT = 0xCF;    // STX opr16a
constexpr uint32_t STX_IX2 = 0xDF;    // STX oprx16,X
constexpr uint32_t STX_IX1 = 0xEF;    // STX oprx8,X
constexpr uint32_t STX_IX = 0xFF;     // STX ,X
constexpr uint32_t STX_SP2 = 0x9EDF;  // STX oprx16,SP
constexpr uint32_t STX_SP1 = 0x9EEF;  // STX oprx8,SP

// SUB - Subtract
constexpr uint32_t SUB_IMM = 0xA0;    // SUB #opr8i
constexpr uint32_t SUB_DIR = 0xB0;    // SUB opr8a
constexpr uint32_t SUB_EXT = 0xC0;    // SUB opr16a
constexpr uint32_t SUB_IX2 = 0xD0;    // SUB oprx16,X
constexpr uint32_t SUB_IX1 = 0xE0;    // SUB oprx8,X
constexpr uint32_t SUB_IX = 0xF0;     // SUB ,X
constexpr uint32_t SUB_SP2 = 0x9ED0;  // SUB oprx16,SP
constexpr uint32_t SUB_SP1 = 0x9EE0;  // SUB oprx8,SP

// SWI - Software interrupt
constexpr uint32_t SWI_INH = 0x83;  // SWI

// TAP - Transfer accumulator to processor status byte
constexpr uint32_t TAP_INH = 0x84;  // TAP

// TAX - Transfer accumulator to X (index register low)
constexpr uint32_t TAX_INH = 0x97;  // TAX

// TPA - Transfer processor status byte to accumulator
constexpr uint32_t TPA_INH = 0x85;  // TPA

// TST - Test for negative or zero
constexpr uint32_t TST_DIR = 0x3D;    // TST opr8a
constexpr uint32_t TSTA_INH = 0x4D;   // TSTA
constexpr uint32_t TSTX_INH = 0x5D;   // TSTX
constexpr uint32_t TST_IX1 = 0x6D;    // TST oprx8,X
constexpr uint32_t TST_IX = 0x7D;     // TST ,X
constexpr uint32_t TST_SP1 = 0x9E6D;  // TST oprx8,SP

// TSX - Transfer stack pointer to index register
constexpr uint32_t TSX_INH = 0x95;  // TSX

// TXA - Transfer X (index register low) to accumulator
constexpr uint32_t TXA_INH = 0x9F;  // TXA

// TXS - Transfer index register to stack pointer
constexpr uint32_t TXS_INH = 0x94;  // TXS

// WAIT - Enable interrupts; stop processor
constexpr uint32_t WAIT_INH = 0x8F;  // WAIT

// Map of opcode to name; used for testing
static const std::unordered_map<uint16_t, std::string> NAMES = {
    {ADC_IMM, "adc"},          {ADC_DIR, "adc"},
    {ADC_EXT, "adc"},          {ADC_IX2, "adc"},
    {ADC_IX1, "adc"},          {ADC_IX, "adc"},
    {ADC_SP2, "adc"},          {ADC_SP1, "adc"},
    {ADD_IMM, "add"},          {ADD_DIR, "add"},
    {ADD_EXT, "add"},          {ADD_IX2, "add"},
    {ADD_IX1, "add"},          {ADD_IX, "add"},
    {ADD_SP2, "add"},          {ADD_SP1, "add"},
    {AIS_IMM, "ais"},          {AIX_IMM, "aix"},
    {AND_IMM, "and"},          {AND_DIR, "and"},
    {AND_EXT, "and"},          {AND_IX2, "and"},
    {AND_IX1, "and"},          {AND_IX, "and"},
    {AND_SP2, "and"},          {AND_SP1, "and"},
    {ASR_DIR, "asr"},          {ASRA_INH, "asra"},
    {ASRX_INH, "asrx"},        {ASR_IX1, "asr"},
    {ASR_IX, "asr"},           {ASR_SP1, "asr"},
    {BCC_BHS_REL, "bcc"},      {BCLR_DIR_B0, "bclr"},
    {BCLR_DIR_B1, "bclr"},     {BCLR_DIR_B2, "bclr"},
    {BCLR_DIR_B3, "bclr"},     {BCLR_DIR_B4, "bclr"},
    {BCLR_DIR_B5, "bclr"},     {BCLR_DIR_B6, "bclr"},
    {BCLR_DIR_B7, "bclr"},     {BCS_BLO_REL, "bcs"},
    {BEQ_REL, "beq"},          {BGE_REL, "bge"},
    {BGND_INH, "bgnd"},        {BGT_REL, "bgt"},
    {BHCC_REL, "bhcc"},        {BHCS_REL, "bhcs"},
    {BHI_REL, "bhi"},          {BIH_REL, "bih"},
    {BIL_REL, "bil"},          {BIT_IMM, "bit"},
    {BIT_DIR, "bit"},          {BIT_EXT, "bit"},
    {BIT_IX2, "bit"},          {BIT_IX1, "bit"},
    {BIT_IX, "bit"},           {BIT_SP2, "bit"},
    {BIT_SP1, "bit"},          {BLE_REL, "ble"},
    {BLS_REL, "bls"},          {BLT_REL, "blt"},
    {BMC_REL, "bmc"},          {BMI_REL, "bmi"},
    {BMS_REL, "bms"},          {BNE_REL, "bne"},
    {BPL_REL, "bpl"},          {BRA_REL, "bra"},
    {BRCLR_DIR_B0, "brclr"},   {BRCLR_DIR_B1, "brclr"},
    {BRCLR_DIR_B2, "brclr"},   {BRCLR_DIR_B3, "brclr"},
    {BRCLR_DIR_B4, "brclr"},   {BRCLR_DIR_B5, "brclr"},
    {BRCLR_DIR_B6, "brclr"},   {BRCLR_DIR_B7, "brclr"},
    {BRN_REL, "brn"},          {BRSET_DIR_B0, "brset"},
    {BRSET_DIR_B1, "brset"},   {BRSET_DIR_B2, "brset"},
    {BRSET_DIR_B3, "brset"},   {BRSET_DIR_B4, "brset"},
    {BRSET_DIR_B5, "brset"},   {BRSET_DIR_B6, "brset"},
    {BRSET_DIR_B7, "brset"},   {BSET_DIR_B0, "bset"},
    {BSET_DIR_B1, "bset"},     {BSET_DIR_B2, "bset"},
    {BSET_DIR_B3, "bset"},     {BSET_DIR_B4, "bset"},
    {BSET_DIR_B5, "bset"},     {BSET_DIR_B6, "bset"},
    {BSET_DIR_B7, "bset"},     {BSR_REL, "bsr"},
    {CBEQ_DIR, "cbeq"},        {CBEQA_IMM, "cbeqa"},
    {CBEQX_IMM, "cbeqx"},      {CBEQ_IX1_POSTINC, "cbeq"},
    {CBEQ_IX_POSTINC, "cbeq"}, {CBEQ_SP1, "cbeq"},
    {CLC_INH, "clc"},          {CLI_INH, "cli"},
    {CLR_DIR, "clr"},          {CLRA_INH, "clra"},
    {CLRX_INH, "clrx"},        {CLRH_INH, "clrh"},
    {CLR_IX1, "clr"},          {CLR_IX, "clr"},
    {CLR_SP1, "clr"},          {CMP_IMM, "cmp"},
    {CMP_DIR, "cmp"},          {CMP_EXT, "cmp"},
    {CMP_IX2, "cmp"},          {CMP_IX1, "cmp"},
    {CMP_IX, "cmp"},           {CMP_SP2, "cmp"},
    {CMP_SP1, "cmp"},          {COM_DIR, "com"},
    {COMA_INH, "coma"},        {COMX_INH, "comx"},
    {COM_IX1, "com"},          {COM_IX, "com"},
    {COM_SP1, "com"},          {CPHX_EXT, "cphx"},
    {CPHX_IMM, "cphx"},        {CPHX_DIR, "cphx"},
    {CPHX_SP1, "cphx"},        {CPX_IMM, "cpx"},
    {CPX_DIR, "cpx"},          {CPX_EXT, "cpx"},
    {CPX_IX2, "cpx"},          {CPX_IX1, "cpx"},
    {CPX_IX, "cpx"},           {CPX_SP2, "cpx"},
    {CPX_SP1, "cpx"},          {DAA_INH, "daa"},
    {DBNZ_DIR, "dbnz"},        {DBNZA_INH, "dbnza"},
    {DBNZX_INH, "dbnzx"},      {DBNZ_IX1, "dbnz"},
    {DBNZ_IX, "dbnz"},         {DBNZ_SP1, "dbnz"},
    {DEC_DIR, "dec"},          {DECA_INH, "deca"},
    {DECX_INH, "decx"},        {DEC_IX1, "dec"},
    {DEC_IX, "dec"},           {DEC_SP1, "dec"},
    {DIV_INH, "div"},          {EOR_IMM, "eor"},
    {EOR_DIR, "eor"},          {EOR_EXT, "eor"},
    {EOR_IX2, "eor"},          {EOR_IX1, "eor"},
    {EOR_IX, "eor"},           {EOR_SP2, "eor"},
    {EOR_SP1, "eor"},          {INC_DIR, "inc"},
    {INCA_INH, "inca"},        {INCX_INH, "incx"},
    {INC_IX1, "inc"},          {INC_IX, "inc"},
    {INC_SP1, "inc"},          {JMP_DIR, "jmp"},
    {JMP_EXT, "jmp"},          {JMP_IX2, "jmp"},
    {JMP_IX1, "jmp"},          {JMP_IX, "jmp"},
    {JSR_DIR, "jsr"},          {JSR_EXT, "jsr"},
    {JSR_IX2, "jsr"},          {JSR_IX1, "jsr"},
    {JSR_IX, "jsr"},           {LDA_IMM, "lda"},
    {LDA_DIR, "lda"},          {LDA_EXT, "lda"},
    {LDA_IX2, "lda"},          {LDA_IX1, "lda"},
    {LDA_IX, "lda"},           {LDA_SP2, "lda"},
    {LDA_SP1, "lda"},          {LDHX_IMM, "ldhx"},
    {LDHX_DIR, "ldhx"},        {LDHX_EXT, "ldhx"},
    {LDHX_IX, "ldhx"},         {LDHX_IX2, "ldhx"},
    {LDHX_IX1, "ldhx"},        {LDHX_SP1, "ldhx"},
    {LDX_IMM, "ldx"},          {LDX_DIR, "ldx"},
    {LDX_EXT, "ldx"},          {LDX_IX2, "ldx"},
    {LDX_IX1, "ldx"},          {LDX_IX, "ldx"},
    {LDX_SP2, "ldx"},          {LDX_SP1, "ldx"},
    {LSL_DIR, "lsl"},          {LSLA_INH, "lsla"},
    {LSLX_INH, "lslx"},        {LSL_IX1, "lsl"},
    {LSL_IX, "lsl"},           {LSL_SP1, "lsl"},
    {LSR_DIR, "lsr"},          {LSRA_INH, "lsra"},
    {LSRX_INH, "lsrx"},        {LSR_IX1, "lsr"},
    {LSR_IX, "lsr"},           {LSR_SP1, "lsr"},
    {MOV_DIR_DIR, "mov"},      {MOV_DIR_IX_POSTINC, "mov"},
    {MOV_IMM_DIR, "mov"},      {MOV_IX_POSTINC_DIR, "mov"},
    {MUL_INH, "mul"},          {NEG_DIR, "neg"},
    {NEGA_INH, "nega"},        {NEGX_INH, "negx"},
    {NEG_IX1, "neg"},          {NEG_IX, "neg"},
    {NEG_SP1, "neg"},          {NOP_INH, "nop"},
    {NSA_INH, "nsa"},          {ORA_IMM, "ora"},
    {ORA_DIR, "ora"},          {ORA_EXT, "ora"},
    {ORA_IX2, "ora"},          {ORA_IX1, "ora"},
    {ORA_IX, "ora"},           {ORA_SP2, "ora"},
    {ORA_SP1, "ora"},          {PSHA_INH, "psha"},
    {PSHH_INH, "pshh"},        {PSHX_INH, "pshx"},
    {PULA_INH, "pula"},        {PULH_INH, "pulh"},
    {PULX_INH, "pulx"},        {ROL_DIR, "rol"},
    {ROLA_INH, "rola"},        {ROLX_INH, "rolx"},
    {ROL_IX1, "rol"},          {ROL_IX, "rol"},
    {ROL_SP1, "rol"},          {ROR_DIR, "ror"},
    {RORA_INH, "rora"},        {RORX_INH, "rorx"},
    {ROR_IX1, "ror"},          {ROR_IX, "ror"},
    {ROR_SP1, "ror"},          {RSP_INH, "rsp"},
    {RTI_INH, "rti"},          {RTS_INH, "rts"},
    {SBC_IMM, "sbc"},          {SBC_DIR, "sbc"},
    {SBC_EXT, "sbc"},          {SBC_IX2, "sbc"},
    {SBC_IX1, "sbc"},          {SBC_IX, "sbc"},
    {SBC_SP2, "sbc"},          {SBC_SP1, "sbc"},
    {SEC_INH, "sec"},          {SEI_INH, "sei"},
    {STA_DIR, "sta"},          {STA_EXT, "sta"},
    {STA_IX2, "sta"},          {STA_IX1, "sta"},
    {STA_IX, "sta"},           {STA_SP2, "sta"},
    {STA_SP1, "sta"},          {STHX_DIR, "sthx"},
    {STHX_EXT, "sthx"},        {STHX_SP1, "sthx"},
    {STOP_INH, "stop"},        {STX_DIR, "stx"},
    {STX_EXT, "stx"},          {STX_IX2, "stx"},
    {STX_IX1, "stx"},          {STX_IX, "stx"},
    {STX_SP2, "stx"},          {STX_SP1, "stx"},
    {SUB_IMM, "sub"},          {SUB_DIR, "sub"},
    {SUB_EXT, "sub"},          {SUB_IX2, "sub"},
    {SUB_IX1, "sub"},          {SUB_IX, "sub"},
    {SUB_SP2, "sub"},          {SUB_SP1, "sub"},
    {SWI_INH, "swi"},          {TAP_INH, "tap"},
    {TAX_INH, "tax"},          {TPA_INH, "tpa"},
    {TST_DIR, "tst"},          {TSTA_INH, "tsta"},
    {TSTX_INH, "tstx"},        {TST_IX1, "tst"},
    {TST_IX, "tst"},           {TST_SP1, "tst"},
    {TSX_INH, "tsx"},          {TXA_INH, "txa"},
    {TXS_INH, "txs"},          {WAIT_INH, "wait"},
};

// A set of only branching instructions; used for testing
static const std::set BRANCHING = {
    BRA_REL,          BEQ_REL,      BNE_REL,      BCC_BHS_REL,  BCS_BLO_REL,
    BPL_REL,          BMI_REL,      BIL_REL,      BIH_REL,      BMC_REL,
    BMS_REL,          BHCC_REL,     BHCS_REL,     BLT_REL,      BLE_REL,
    BGE_REL,          BGT_REL,      BLS_REL,      BHI_REL,      BRSET_DIR_B0,
    BRSET_DIR_B1,     BRSET_DIR_B2, BRSET_DIR_B3, BRSET_DIR_B4, BRSET_DIR_B5,
    BRSET_DIR_B6,     BRSET_DIR_B7, BRCLR_DIR_B0, BRCLR_DIR_B1, BRCLR_DIR_B2,
    BRCLR_DIR_B3,     BRCLR_DIR_B4, BRCLR_DIR_B5, BRCLR_DIR_B6, BRCLR_DIR_B7,
    BSR_REL,          CBEQ_DIR,     CBEQA_IMM,    CBEQX_IMM,    CBEQ_IX_POSTINC,
    CBEQ_IX1_POSTINC, CBEQ_SP1,     DBNZ_DIR,     DBNZA_INH,    DBNZX_INH,
    DBNZ_IX,          DBNZ_IX1,     DBNZ_SP1,     JMP_DIR,      JMP_EXT,
    JMP_IX,           JMP_IX1,      JMP_IX2,      JSR_DIR,      JSR_EXT,
    JSR_IX,           JSR_IX1,      JSR_IX2,      RTS_INH,      RTI_INH};
}  // namespace HCS08::Opcodes

namespace HCS08::OpcodeFields {
/* ----------------------------------------- */
/*  ADDITIONAL MASKS AND VALUES FOR PARSING  */
/* ----------------------------------------- */
constexpr uint32_t EXT_OP = 0x9E;  // First byte of all 2-byte opcodes
}  // namespace HCS08::OpcodeFields

#endif  // HCS08_OPCODES_H
