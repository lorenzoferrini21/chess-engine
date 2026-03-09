#pragma once

#include "../board/board.h"

class Evaluate
{
public:
    static int evaluate(const Board& board);

    static constexpr int pieceValue[PIECE_TYPE_NB] =
{
    100,  // pawn
    320,  // knight
    330,  // bishop
    500,  // rook
    900,  // queen
    20000 // king
};
};