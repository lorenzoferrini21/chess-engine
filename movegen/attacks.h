#pragma once

#include "../core/type.h"

namespace Attacks {

    extern Bitboard knightAttacks[64];
    extern Bitboard kingAttacks[64];

    void init();

    Bitboard getRookAttacks(Square sq, Bitboard occupancy);
    Bitboard getBishopAttacks(Square sq, Bitboard occupancy);
}
