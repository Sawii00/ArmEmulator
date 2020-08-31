#pragma once
#include <iostream>
#include "types.h"
#define ERROR(string): ASSERT(1, string)

void ASSERT(bool condition, const char* message);
DWORD rotl32(DWORD value, u8 count);
DWORD rotr32(DWORD value, u8 count);
DWORD asr32(DWORD value, u8 count);
