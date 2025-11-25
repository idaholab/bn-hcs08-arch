// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef HCS08_UTIL_H
#define HCS08_UTIL_H

#include <cstdint>

namespace HCS08 {
const char* FlagToStr(uint32_t flag_id);
const char* RegToStr(uint8_t reg_id);

}  // namespace HCS08

#endif  // HCS08_UTIL_H
