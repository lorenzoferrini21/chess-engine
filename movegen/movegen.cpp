#include "movegen.h"
#include "attacks.h"
#include "../core/bitboard.h"
#include "../move/makemove.h"

static const Bitboard FILE_A = 0x0101010101010101ULL;
static const Bitboard FILE_H = 0x8080808080808080ULL;

static const Bitboard RANK_2 = 0x000000000000FF00ULL;
static const Bitboard RANK_7 = 0x00FF000000000000ULL;

static const Bitboard RANK_1 = 0x00000000000000FFULL;
static const Bitboard RANK_8 = 0xFF00000000000000ULL;

// ------------------------------------------------------------

Move MoveGen::encodeMove(Square from, Square to, int promotedPiece) {
    return  (from & 0x3F) | ((to & 0x3F) << 6) | ((promotedPiece & 0xF) << 12);
}

// ------------------------------------------------------------

//-------------------------------------------------------------
// LOGICA PEDONI
//-------------------------------------------------------------

void MoveGen::generatePawnMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;

    Bitboard pawns = board.pieces[side][PAWN];
    Bitboard empty = ~board.allOccupancy;

    if (side == WHITE)
    {
        // ========================
        // SPINTA SINGOLA
        // ========================
        Bitboard singlePush = (pawns << 8) & empty;

        Bitboard promotions = singlePush & RANK_8;
        Bitboard normalPush = singlePush & ~RANK_8;

        // mosse normali
        Bitboard temp = normalPush;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 8;
            moves.push_back(encodeMove(from, to));
        }

        // promozioni
        temp = promotions;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 8;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

        // ========================
        // SPINTA DOPPIA
        // ========================
        Bitboard doublePush = ((pawns & RANK_2) << 8) & empty;
        doublePush = (doublePush << 8) & empty;

        temp = doublePush;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 16;

            moves.push_back(encodeMove(from, to));
        }

        // ========================
		// CATTURE NORMALI E CATTURA CON PROMOZIONE
        // ========================

        // diagonale sinistra
        Bitboard captureLeft = ((pawns & ~FILE_A) << 7) & board.occupancy[BLACK];

        Bitboard promoLeft = captureLeft & RANK_8;
        Bitboard normalLeft = captureLeft & ~RANK_8;

        // normali
        temp = normalLeft;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 7;
            moves.push_back(encodeMove(from, to));
        }

        // promozioni
        temp = promoLeft;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 7;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

        // diagonale destra
        Bitboard captureRight = ((pawns & ~FILE_H) << 9) & board.occupancy[BLACK];

        Bitboard promoRight = captureRight & RANK_8;
        Bitboard normalRight = captureRight & ~RANK_8;

        // normali
        temp = normalRight;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 9;
            moves.push_back(encodeMove(from, to));
        }

        // promozioni
        temp = promoRight;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to - 9;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

        // ========================
        // EN PASSANT
        // ========================
        if (board.enPassantSquare != -1)
        {
            Bitboard epSquareBB = 1ULL << board.enPassantSquare;

            Bitboard epLeft =
                ((pawns & ~FILE_A) << 7) & epSquareBB;

            Bitboard epRight =
                ((pawns & ~FILE_H) << 9) & epSquareBB;

            temp = epLeft;
            while (temp)
            {
                Square to = popLSB(temp);
                Square from = to - 7;
                moves.push_back(encodeMove(from, to));
            }

            temp = epRight;
            while (temp)
            {
                Square to = popLSB(temp);
                Square from = to - 9;
                moves.push_back(encodeMove(from, to));
            }
        }
    }
    else // ================= BLACK =================
    {
        // ========================
        // SPINTA SINGOLA
        // ========================
        Bitboard singlePush = (pawns >> 8) & empty;

        Bitboard promotions = singlePush & RANK_1;
        Bitboard normalPush = singlePush & ~RANK_1;

        // mosse normali
        Bitboard temp = normalPush;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 8;
            moves.push_back(encodeMove(from, to));
        }

        // promozioni
        temp = promotions;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 8;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

        // ========================
        // SPINTA DOPPIA
        // ========================
        Bitboard doublePush = ((pawns & RANK_7) >> 8) & empty;
        doublePush = (doublePush >> 8) & empty;

        temp = doublePush;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 16;

            moves.push_back(encodeMove(from, to));
        }

        // ========================
		// CATTURE NORMALI E CATTURA CON PROMOZIONE
        // ========================

        //DIAGONALE DESTRA
        Bitboard captureLeft =
            ((pawns & ~FILE_H) >> 7) & board.occupancy[WHITE];

        Bitboard promoLeft = captureLeft & RANK_1;
        Bitboard normalLeft = captureLeft & ~RANK_1;

		// CATTURE NORMALI

        temp = normalLeft;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 7;

            moves.push_back(encodeMove(from, to));
        }

		// CATTURE CON PROMOZIONE

        temp = promoLeft;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 7;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

		//DIAGONALE SINISTRA
        Bitboard captureRight =
            ((pawns & ~FILE_A) >> 9) & board.occupancy[WHITE];

        Bitboard promoRight = captureRight & RANK_1;
        Bitboard normalRight = captureRight & ~RANK_1;

		// CATTURE NORMALI
        temp = normalRight;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 9;

            moves.push_back(encodeMove(from, to));
        }
        
		// CATTURE CON PROMOZIONE
        temp = promoRight;
        while (temp)
        {
            Square to = popLSB(temp);
            Square from = to + 9;

            moves.push_back(encodeMove(from, to, QUEEN));
            moves.push_back(encodeMove(from, to, ROOK));
            moves.push_back(encodeMove(from, to, BISHOP));
            moves.push_back(encodeMove(from, to, KNIGHT));
        }

        // ========================
        // EN PASSANT
        // ========================
        if (board.enPassantSquare != -1)
        {
            Bitboard epSquareBB = 1ULL << board.enPassantSquare;

            Bitboard epLeft =
                ((pawns & ~FILE_H) >> 7) & epSquareBB;

            Bitboard epRight =
                ((pawns & ~FILE_A) >> 9) & epSquareBB;

            temp = epLeft;
            while (temp)
            {
                Square to = popLSB(temp);
                Square from = to + 7;
                moves.push_back(encodeMove(from, to));
            }

            temp = epRight;
            while (temp)
            {
                Square to = popLSB(temp);
                Square from = to + 9;
                moves.push_back(encodeMove(from, to));
            }
        }
    }
}
//-------------------------------------------------------------
// LOGICA CAVALLO
//-------------------------------------------------------------

void MoveGen::generateKnightMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;
    Bitboard knights = board.pieces[side][KNIGHT];

    while (knights)
    {
        Square from = popLSB(knights);

        Bitboard attacks = Attacks::knightAttacks[from];

        // Non possiamo mangiare i nostri pezzi
        attacks &= ~board.occupancy[side];

        Bitboard temp = attacks;

        while (temp)
        {
            Square to = popLSB(temp);

            moves.push_back(encodeMove(from, to));

        }
    }
}
//-------------------------------------------------------------
// LOGICA ALFIERE
//-------------------------------------------------------------
void MoveGen::generateBishopMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;
    Bitboard bishop = board.pieces[side][BISHOP];

    while (bishop)
    {
        Square from = popLSB(bishop);

        Bitboard attacks =
            Attacks::getBishopAttacks(from, board.allOccupancy);

        attacks &= ~board.occupancy[side];

        while (attacks)
        {
            Square to = popLSB(attacks);
            moves.push_back(encodeMove(from, to));
        }
    }
}

//-------------------------------------------------------------
// LOGICA TORRE
//-------------------------------------------------------------
void MoveGen::generateRookMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;
    Bitboard rooks = board.pieces[side][ROOK];

    while (rooks)
    {
        Square from = popLSB(rooks);

        Bitboard attacks =
            Attacks::getRookAttacks(from, board.allOccupancy);

        attacks &= ~board.occupancy[side];

        while (attacks)
        {
            Square to = popLSB(attacks);
            moves.push_back(encodeMove(from, to));
        }
    }
}
//-------------------------------------------------------------
// LOGICA REGINA
//-------------------------------------------------------------
void MoveGen::generateQueenMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;
    Bitboard queen = board.pieces[side][QUEEN];

    while (queen)
    {
        Square from = popLSB(queen);

        Bitboard attacks =
            Attacks::getRookAttacks(from, board.allOccupancy) |
            Attacks::getBishopAttacks(from, board.allOccupancy);

        attacks &= ~board.occupancy[side];

        while (attacks)
        {
            Square to = popLSB(attacks);
            moves.push_back(encodeMove(from, to));
        }
    }
}

//-------------------------------------------------------------
// LOGICA RE
//-------------------------------------------------------------
void MoveGen::generateKingMoves(const Board& board, std::vector<Move>& moves)
{
    Color side = board.sideToMove;

    Bitboard king = board.pieces[side][KING];

    while (king)
    {
        Square from = popLSB(king);

        Bitboard attacks = Attacks::kingAttacks[from];

        // Non possiamo mangiare i nostri pezzi
        attacks &= ~board.occupancy[side];

        while (attacks)
        {
            Square to = popLSB(attacks);
            moves.push_back(encodeMove(from, to));
        }
    }
	//ARROCCO BIANCO
    if (board.sideToMove == WHITE)
    {
        // King side (e1g1)
        if (board.castlingRights[WHITE][0])
        {
            if (!(board.allOccupancy & ((1ULL << F1) | (1ULL << G1))))
            {
                if (!isSquareAttacked(board, E1, BLACK) &&
                    !isSquareAttacked(board, F1, BLACK) &&
                    !isSquareAttacked(board, G1, BLACK))
                {
                    moves.push_back(encodeMove(E1, G1));
                }
            }
        }

        // Queen side (e1c1)
        if (board.castlingRights[WHITE][1])
        {
            if (!(board.allOccupancy & ((1ULL << D1) | (1ULL << C1) | (1ULL << B1))))
            {
                if (!isSquareAttacked(board, E1, BLACK) &&
                    !isSquareAttacked(board, D1, BLACK) &&
                    !isSquareAttacked(board, C1, BLACK))
                {
                    moves.push_back(encodeMove(E1, C1));
                }
            }
        }
    }
    //ARROCCO NERO
    if (board.sideToMove == BLACK)
    {
        // King side (e8g8)
        if (board.castlingRights[BLACK][0])
        {
            if (!(board.allOccupancy & ((1ULL << F8) | (1ULL << G8))))
            {
                if (!isSquareAttacked(board, E8, WHITE) &&
                    !isSquareAttacked(board, F8, WHITE) &&
                    !isSquareAttacked(board, G8, WHITE))
                {
                    moves.push_back(encodeMove(E8, G8));
                }
            }
        }

        // Queen side (e8c8)
        if (board.castlingRights[BLACK][1])
        {
            if (!(board.allOccupancy & ((1ULL << D8) | (1ULL << C8) | (1ULL << B8))))
            {
                if (!isSquareAttacked(board, E8, WHITE) &&
                    !isSquareAttacked(board, D8, WHITE) &&
                    !isSquareAttacked(board, C8, WHITE))
                {
                    moves.push_back(encodeMove(E8, C8));
                }
            }
        }
    }
}
//-------------------------------------------------------------
//FUNZIONE CHE CONTROLLA SE UNA CASELLA è ATTACCATA
//-------------------------------------------------------------
bool MoveGen::isSquareAttacked(const Board& board,
    Square sq,
    Color bySide)
{
    Bitboard target = (1ULL << sq);

    // ==========================
    // 1. Pawn attacks
    // ==========================
    if (bySide == WHITE)
    {
        Bitboard pawns = board.pieces[WHITE][PAWN];

        // pedoni bianchi attaccano NE e NW
        Bitboard attacks =
            ((pawns << 7) & 0xFEFEFEFEFEFEFEFEULL) |
            ((pawns << 9) & 0x7F7F7F7F7F7F7F7FULL);

        if (attacks & target)
            return true;
    }
    else
    {
        Bitboard pawns = board.pieces[BLACK][PAWN];

        // pedoni neri attaccano SE e SW
        Bitboard attacks =
            ((pawns >> 7) & 0x7F7F7F7F7F7F7F7FULL) |
            ((pawns >> 9) & 0xFEFEFEFEFEFEFEFEULL);

        if (attacks & target)
            return true;
    }

    // ==========================
    // 2. Knight attacks
    // ==========================
    if (Attacks::knightAttacks[sq] &
        board.pieces[bySide][KNIGHT])
        return true;

    // ==========================
    // 3. Bishop / Queen (diagonali)
    // ==========================
    Bitboard bishopAttacks =
        Attacks::getBishopAttacks(sq, board.allOccupancy);

    if (bishopAttacks &
        (board.pieces[bySide][BISHOP] |
            board.pieces[bySide][QUEEN]))
        return true;

    // ==========================
    // 4. Rook / Queen (linee rette)
    // ==========================
    Bitboard rookAttacks =
        Attacks::getRookAttacks(sq, board.allOccupancy);

    if (rookAttacks &
        (board.pieces[bySide][ROOK] |
            board.pieces[bySide][QUEEN]))
        return true;

    // ==========================
    // 5. King
    // ==========================
    if (Attacks::kingAttacks[sq] &
        board.pieces[bySide][KING])
        return true;

    return false;
}
// -------------------------------------------------------------
//CONTROLLA SE IL RE DI UN COLORE è IN SCACCO
// -------------------------------------------------------------
bool MoveGen::isInCheck(const Board& board, Color side)
{
    // Trova il re
    Bitboard kingBB = board.pieces[side][KING];

    if (!kingBB)
        return false; // sicurezza, non dovrebbe mai succedere

    Bitboard copy = kingBB;
    Square kingSquare = popLSB(copy);

    // Colore avversario
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    return isSquareAttacked(board, kingSquare, enemy);
}

void MoveGen::generateLegalMoves(Board& board, std::vector<Move>& legalMoves)
{
    std::vector<Move> pseudoMoves;
    generateAllMoves(board, pseudoMoves);

    Color side = board.sideToMove;

    legalMoves.clear();

    for (Move m : pseudoMoves)
    {
        MakeMove::makeMove(board, m);

        // Dopo makeMove il lato è cambiato.
        // Dobbiamo controllare se il lato originale è in check.
        if (!isInCheck(board, side))
        {
            legalMoves.push_back(m);
        }

        MakeMove::undoMove(board);
    }
}

void MoveGen::generateAllMoves(Board& board, std::vector<Move>& moves)
{
    moves.clear();

    generatePawnMoves(board, moves);
    generateKnightMoves(board, moves);
    generateBishopMoves(board, moves);
    generateRookMoves(board, moves);
    generateQueenMoves(board, moves);
    generateKingMoves(board, moves);
}
