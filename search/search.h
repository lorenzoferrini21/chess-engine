#pragma once

#include "../board/board.h"
#include "../core/type.h"

class Search {
public:

    static Move findBestMove(Board& board, int depth);
    static void initMVVLVA();

private:

    static int alphaBeta(Board& board, int depth, int alpha, int beta);

};