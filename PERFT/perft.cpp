#include "perft.h"
#include "../move/makemove.h"
#include "../movegen/movegen.h"

uint64_t Perft::run(Board& board, int depth)
{
    if (depth == 0)
        return 1ULL;

    std::vector<Move> moves;
    MoveGen::generateLegalMoves(board, moves);

    uint64_t nodes = 0;

    for (Move m : moves)
    {
        MakeMove::makeMove(board, m);

        nodes += run(board, depth - 1);

        MakeMove::undoMove(board);
    }

    return nodes;
}