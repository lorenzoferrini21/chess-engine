
#pragma once

#include "type.h"

struct UndoState
{
    Move move;

    PieceType movedPiece;
    PieceType capturedPiece;

    int enPassantSquare;

    bool castlingRights[2][2];

    int halfmoveClock;
    int fullmoveNumber;
};