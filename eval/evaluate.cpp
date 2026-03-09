#include "evaluate.h"
#include "../core/type.h"
#include <bit>

static const int pieceValue[PIECE_TYPE_NB] =
{
    100,   // pawn
    320,   // knight
    330,   // bishop
    500,   // rook
    900,   // queen
    20000  // king
};

int Evaluate::evaluate(const Board& board)
{
    int score = 0;

    for (int piece = 0; piece < PIECE_TYPE_NB; piece++)
    {
        Bitboard whiteBB = board.pieces[WHITE][piece];
        Bitboard blackBB = board.pieces[BLACK][piece];

        int whiteCount = std::popcount(whiteBB);
        int blackCount = std::popcount(blackBB);

        score += pieceValue[piece] * whiteCount;
        score -= pieceValue[piece] * blackCount;
    }

    // Se è il turno del nero invertiamo il punteggio
    if (board.sideToMove == BLACK)
        score = -score;

    return score;
}