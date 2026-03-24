#include "search.h"
#include "../movegen/movegen.h"
#include "../move/makemove.h"
#include "../eval/evaluate.h"

#include <vector>
#include <limits>
#include <algorithm>

static const int INF = 100000;

static int mvvLva[PIECE_TYPE_NB][PIECE_TYPE_NB];

Move Search::bestMove = 0;

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

Move Search::findBestMove(Board& board, int maxDepth)
{
    Move best = 0;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        int score = alphaBeta(board, depth, -INF, INF, true);

        best = bestMove;

        std::cout << "info depth "
                  << depth
                  << " score "
                  << score
                  << std::endl;
    }

    return best;
}

int Search::alphaBeta(Board& board, int depth, int alpha, int beta, bool root)
{
    if (depth == 0)
        return quiescence(board, alpha, beta);

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
            return -INF + depth; // checkmate (depth bonus)
        else
            return 0;
    }

    for (Move move : moves)
    {
        MakeMove::makeMove(board, move);

        int score = -alphaBeta(board, depth - 1, -beta, -alpha, false);

        MakeMove::undoMove(board);

        if (score >= beta)
            return beta;

        if (score > alpha)
        {
            alpha = score;

            if (root)
                bestMove = move;
        }
    }

    return alpha;
}

int Search::quiescence(Board& board, int alpha, int beta)
{
    int stand_pat = Evaluate::evaluate(board);

    if (stand_pat >= beta)
        return beta;

    if (alpha < stand_pat)
        alpha = stand_pat;

    std::vector<Move> moves;
    MoveGen::generateAllMoves(board, moves);

    for (Move move : moves)
    {
        if (!board.isCapture(move))
            continue;

        MakeMove::makeMove(board, move);

        int score = -quiescence(board, -beta, -alpha);

        MakeMove::undoMove(board);

        if (score >= beta)
            return beta;

        if (score > alpha)
            alpha = score;
    }

    return alpha;
}

