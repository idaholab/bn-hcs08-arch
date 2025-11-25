// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef HCS08_FLAGS_H
#define HCS08_FLAGS_H

namespace HCS08::Flags {
/* ---------------------------------------- */
/*  Flags in Condition Code Register (CCR)  */
/* ---------------------------------------- */
/*   7   6   5   4   3   2   1   0          */
/* [ V | 1 | 1 | H | I | N | Z | C ]        */
/* ---------------------------------------- */
/*   V: Two's complement overflow           */
/*               H: Half-carry (from bit 3) */
/*                   I: Interrupt mask      */
/*                       N: Negative        */
/*                           Z: Zero        */
/*                               C: Carry   */
/* ---------------------------------------- */
constexpr uint8_t FLAG_C = 0;  // Carry
constexpr uint8_t FLAG_Z = 1;  // Zero
constexpr uint8_t FLAG_N = 2;  // Negative
constexpr uint8_t FLAG_I = 3;  // Interrupt
constexpr uint8_t FLAG_H = 4;  // Half-carry (from bit 3)
constexpr uint8_t FLAG_V = 7;  // Two's complement overflow

/* -------------- */
/* Flag groupings */
/* -------------- */
constexpr uint8_t FLAGS_Z_N_V = 0;
constexpr uint8_t FLAGS_C_Z_N_V = 1;
constexpr uint8_t FLAGS_C_Z_N_H_V = 2;
constexpr uint8_t FLAGS_Z = 3;
constexpr uint8_t FLAGS_C_Z = 4;
}  // namespace HCS08::Flags

#endif  // HCS08_FLAGS_H
