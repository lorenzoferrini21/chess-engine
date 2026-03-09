#pragma once

#include <vector>
#include "../core/type.h"
#include "../board/board.h"

class MoveGen {
public:
    static void generatePawnMoves(const Board& board, std::vector<Move>& moves);

    static void generateKnightMoves(const Board& board, std::vector<Move>& moves);

    static void generateRookMoves(const Board&, std::vector<Move>&);

    static void generateBishopMoves(const Board&, std::vector<Move>&);

    static void generateQueenMoves(const Board&, std::vector<Move>&);

    static void generateKingMoves(const Board&, std::vector<Move>&);

    static void generateAllMoves(Board& board, std::vector<Move>& moves);

    static bool isSquareAttacked(const Board& board, Square sq, Color bySide);
    static bool isInCheck(const Board& board, Color side);

    static void generateLegalMoves(Board& board, std::vector<Move>& legalMoves);
    

private:
    static Move encodeMove(Square from, Square to, int promotedPiece = 0);

    
};
