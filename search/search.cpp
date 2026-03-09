#include "search.h"
#include "../movegen/movegen.h"
#include "../move/makemove.h"
#include "../eval/evaluate.h"

#include <vector>
#include <limits>
#include <algorithm>

static const int INF = 100000;

static int mvvLva[PIECE_TYPE_NB][PIECE_TYPE_NB];

void Search::initMVVLVA()
{
    for (int attacker = PAWN; attacker <= KING; attacker++)
    {
        for (int victim = PAWN; victim <= KING; victim++)
        {
            mvvLva[victim][attacker] =
                Evaluate::pieceValue[victim] + 6 - (Evaluate::pieceValue[attacker] / 100);
        }
    }
}

int scoreMove(Board& board, Move move)
{
    Square to = (move >> 6) & 0x3F;

    PieceType victim = board.pieceOn(to);

    if (victim != PIECE_TYPE_NB)
    {
        Square from = move & 0x3F;
        PieceType attacker = board.pieceOn(from);

        return mvvLva[victim][attacker];
    }

    return 0;
}

Move Search::findBestMove(Board& board, int depth)
{
    std::vector<Move> moves;
    MoveGen::generateLegalMoves(board, moves);

    Move bestMove = 0;
    int bestScore = -INF;

    for (Move m : moves)
    {
        MakeMove::makeMove(board, m);

        int score = -alphaBeta(board, depth - 1, -INF, INF);

        MakeMove::undoMove(board);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = m;
        }
    }

    return bestMove;
}

int Search::alphaBeta(Board& board, int depth, int alpha, int beta)
{
    if (depth == 0)
        return Evaluate::evaluate(board);

    std::vector<Move> moves;
    MoveGen::generateLegalMoves(board, moves);
    std::sort(moves.begin(), moves.end(),
    [&](Move a, Move b)
    {
        return scoreMove(board, a) > scoreMove(board, b);
    });

    if (moves.empty())
    {
        if (MoveGen::isInCheck(board, board.sideToMove))
            return -INF + 1; // checkmate
        else
            return 0; // stalemate
    }

    int bestScore = -INF;

    for (Move move : moves)
    {
        MakeMove::makeMove(board, move);

        int score = -alphaBeta(board, depth - 1, -beta, -alpha);

        MakeMove::undoMove(board);

        if (score >= beta)
            return beta;   // beta cutoff

        if (score > alpha)
            alpha = score; 
    }

    return alpha;

}

