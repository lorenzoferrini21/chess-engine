
#include "makemove.h"



void MakeMove::makeMove(Board& board, Move m)
{
    Square from = m & 0x3F;
    Square to = (m >> 6) & 0x3F;

    Color side = board.sideToMove;
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    UndoState state{};
    state.move = m;

    state.enPassantSquare = board.enPassantSquare;
    state.halfmoveClock = board.halfmoveClock;
    state.fullmoveNumber = board.fullmoveNumber;

    for (int c = 0; c < 2; c++)
        for (int s = 0; s < 2; s++)
            state.castlingRights[c][s] = board.castlingRights[c][s];

    state.capturedPiece = PIECE_TYPE_NB;

    // =========================
    // Trova pezzo che si muove
    // =========================
    PieceType movedPiece = PIECE_TYPE_NB;

    for (int p = 0; p < PIECE_TYPE_NB; p++)
    {
        if (board.pieces[side][p] & (1ULL << from))
        {
            movedPiece = static_cast<PieceType>(p);
            break;
        }
    }

    state.movedPiece = movedPiece;

    if (movedPiece == KING)
    {
        // Arrocco bianco
        if (side == WHITE)
        {
            if (from == E1 && to == G1) // king side
            {
                board.pieces[WHITE][ROOK] &= ~(1ULL << H1);
                board.pieces[WHITE][ROOK] |= (1ULL << F1);
            }
            else if (from == E1 && to == C1) // queen side
            {
                board.pieces[WHITE][ROOK] &= ~(1ULL << A1);
                board.pieces[WHITE][ROOK] |= (1ULL << D1);
            }
        }

        // Arrocco nero
        if (side == BLACK)
        {
            if (from == E8 && to == G8)
            {
                board.pieces[BLACK][ROOK] &= ~(1ULL << H8);
                board.pieces[BLACK][ROOK] |= (1ULL << F8);
            }
            else if (from == E8 && to == C8)
            {
                board.pieces[BLACK][ROOK] &= ~(1ULL << A8);
                board.pieces[BLACK][ROOK] |= (1ULL << D8);
            }
        }
    }
	//PERDITA DI ARROCCO
    if (movedPiece == KING)
    {
        board.castlingRights[side][0] = false;
        board.castlingRights[side][1] = false;
    }
    if (movedPiece == ROOK)
    {
        if (side == WHITE)
        {
            if (from == H1) board.castlingRights[WHITE][0] = false;
            if (from == A1) board.castlingRights[WHITE][1] = false;
        }
        else
        {
            if (from == H8) board.castlingRights[BLACK][0] = false;
            if (from == A8) board.castlingRights[BLACK][1] = false;
        }
    }

    // =========================
    // Rimuovi pezzo dalla casa iniziale
    // =========================
    board.pieces[side][movedPiece] &= ~(1ULL << from);


    // =========================
    // Gestione cattura
    // =========================
    // EN PASSANT capture
    if (movedPiece == PAWN && to == state.enPassantSquare)
    {
        Square capturedPawnSquare;

        if (side == WHITE)
            capturedPawnSquare = to - 8;
        else
            capturedPawnSquare = to + 8;

        board.pieces[enemy][PAWN] &= ~(1ULL << capturedPawnSquare);

        state.capturedPiece = PAWN;
    }
    for (int p = 0; p < PIECE_TYPE_NB; p++)
    {
        if (board.pieces[enemy][p] & (1ULL << to))
        {
            board.pieces[enemy][p] &= ~(1ULL << to);
            state.capturedPiece = static_cast<PieceType>(p);
            break;
        }
    }
    if (state.capturedPiece == ROOK)
    {
        if (enemy == WHITE)
        {
            if (to == H1) board.castlingRights[WHITE][0] = false;
            if (to == A1) board.castlingRights[WHITE][1] = false;
        }
        else
        {
            if (to == H8) board.castlingRights[BLACK][0] = false;
            if (to == A8) board.castlingRights[BLACK][1] = false;
        }
    }

    // =========================
    // Metti pezzo nella nuova casa
    // =========================

	//PROMOZIONE SE SI ARRIVA ALL'ULTIMA RIGA
    int promoted = (m >> 12) & 0xF;

    if (promoted && movedPiece == PAWN)
    {
        // promozione: NON rimettere il pedone
        board.pieces[side][promoted] |= (1ULL << to);
    }
    else
    {
        board.pieces[side][movedPiece] |= (1ULL << to);
    }

    // =========================
    // Reset en passant
    // =========================
    board.enPassantSquare = -1;
    if (movedPiece == PAWN)
    {
        int diff = to - from;

        // doppio passo bianco
        if (side == WHITE && diff == 16)
            board.enPassantSquare = from + 8;

        // doppio passo nero
        if (side == BLACK && diff == -16)
            board.enPassantSquare = from - 8;
    }

    // =========================
    // Halfmove clock
    // =========================
    if (movedPiece == PAWN || state.capturedPiece != PIECE_TYPE_NB)
        board.halfmoveClock = 0;
    else
        board.halfmoveClock++;

    // =========================
    // Fullmove number
    // =========================
    if (side == BLACK)
        board.fullmoveNumber++;

    // =========================
    // Cambia lato
    // =========================
    board.sideToMove = enemy;

    // =========================
    // Aggiorna occupazioni
    // =========================
    board.updateOccupancy();

    // =========================
    // Salva stato nello stack
    // =========================
    board.history.push_back(state);
}

void MakeMove::undoMove(Board& board)
{
    if (board.history.empty())
        return;

    UndoState state = board.history.back();
    board.history.pop_back();

    Move m = state.move;

    Square from = m & 0x3F;
    Square to = (m >> 6) & 0x3F;

    // Ripristina lato
    board.sideToMove = (board.sideToMove == WHITE) ? BLACK : WHITE;

    Color side = board.sideToMove;
    Color enemy = (side == WHITE) ? BLACK : WHITE;
    int promoted = (m >> 12) & 0xF;

    // Riporta il pezzo nella casa originale (gestione promozione)
    if (state.movedPiece == PAWN && promoted)
    {
        board.pieces[side][promoted] &= ~(1ULL << to);
        board.pieces[side][PAWN] |= (1ULL << from);
    }
    else
    {
        board.pieces[side][state.movedPiece] &= ~(1ULL << to);
        board.pieces[side][state.movedPiece] |= (1ULL << from);
    }

    // Ripristina eventuale pezzo catturato (incluso en passant)
    if (state.capturedPiece != PIECE_TYPE_NB)
    {
        if (state.movedPiece == PAWN && to == state.enPassantSquare && state.capturedPiece == PAWN)
        {
            Square capturedPawnSquare = (side == WHITE) ? (to - 8) : (to + 8);
            board.pieces[enemy][PAWN] |= (1ULL << capturedPawnSquare);
        }
        else
        {
            board.pieces[enemy][state.capturedPiece] |= (1ULL << to);
        }
    }

    // Ripristina torre se la mossa annullata era un arrocco
    if (state.movedPiece == KING)
    {
        if (side == WHITE)
        {
            if (from == E1 && to == G1)
            {
                board.pieces[WHITE][ROOK] &= ~(1ULL << F1);
                board.pieces[WHITE][ROOK] |= (1ULL << H1);
            }
            else if (from == E1 && to == C1)
            {
                board.pieces[WHITE][ROOK] &= ~(1ULL << D1);
                board.pieces[WHITE][ROOK] |= (1ULL << A1);
            }
        }
        else
        {
            if (from == E8 && to == G8)
            {
                board.pieces[BLACK][ROOK] &= ~(1ULL << F8);
                board.pieces[BLACK][ROOK] |= (1ULL << H8);
            }
            else if (from == E8 && to == C8)
            {
                board.pieces[BLACK][ROOK] &= ~(1ULL << D8);
                board.pieces[BLACK][ROOK] |= (1ULL << A8);
            }
        }
    }

    // Ripristina stato
    board.enPassantSquare = state.enPassantSquare;
    board.halfmoveClock = state.halfmoveClock;
    board.fullmoveNumber = state.fullmoveNumber;

    for (int c = 0; c < 2; c++)
        for (int s = 0; s < 2; s++)
            board.castlingRights[c][s] = state.castlingRights[c][s];

    board.updateOccupancy();
}
