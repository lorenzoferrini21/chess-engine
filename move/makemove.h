#pragma once
#include "core/type.h"
#include "board/board.h"
#include "core/undo.h"


class MakeMove {
public:
	static void makeMove(Board& board, Move move);
	static void undoMove(Board& board);
private:
};

