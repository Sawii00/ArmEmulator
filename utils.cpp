#include "utils.h"

void ASSERT(bool condition, const char* message)
{
    if(condition)
    {
        std::cerr << message << std::endl;
        exit(1);
    }

}

DWORD rotl32(DWORD value, u8 count)
{
    const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
    count &= mask;
    return (value << count) | (value >> (-count & mask));
}

DWORD rotr32(DWORD value, u8 count)
{
    const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
    count &= mask;
    return (value >> count) | (value << (-count & mask));
}


//@TODO(sawii): test this
DWORD asr32(DWORD value, u8 count)
{
    if(!count)return value;
    //it is negative, we have to add 1s
    if(value >> 31)
    {
        DWORD mask = 0xFFFFFFFF;
        mask = mask << 32 - count;
        value >>= count;
        value |= mask;
        return value;

    }
    else
    {
        return value >> count;
    }

}
