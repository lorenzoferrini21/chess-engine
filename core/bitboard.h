#pragma once
#include <cstdint>
#include "type.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

inline int popLSB(Bitboard& bb)
{
    if (!bb)
        return -1;

#ifdef _MSC_VER
    unsigned long index = 0;
    _BitScanForward64(&index, bb);
    bb &= bb - 1;
    return static_cast<int>(index);
#else
    int index = __builtin_ctzll(bb);
    bb &= bb - 1;
    return index;
#endif
}
