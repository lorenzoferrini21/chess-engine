#pragma once
#include "../board/board.h"


class Perft
{
public:
    static uint64_t run(Board& board, int depth);
};
