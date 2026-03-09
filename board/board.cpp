#include "board.h"
#include <iostream>
#include <bitset>  // utile per stampare bitboard a debug


Board::Board() {
    // Inizializza tutto a zero
    for (int c = 0; c < COLOR_NB; ++c) {
        for (int p = 0; p < PIECE_TYPE_NB; ++p)
            pieces[c][p] = 0ULL; //0=valore, U=unsigned, LL=long long => 64 bit a zero
        occupancy[c] = 0ULL;
        castlingRights[c][0] = castlingRights[c][1] = false;
    }
    allOccupancy = 0ULL;
    enPassantSquare = -1;
    halfmoveClock = 0;
    fullmoveNumber = 1;
    sideToMove = WHITE;
    initStartPosition();
}

// =========================
// Aggiorna occupazione
// =========================
void Board::updateOccupancy() {
    allOccupancy = 0ULL;
    for (int c = 0; c < COLOR_NB; ++c) {
        occupancy[c] = 0ULL;
        for (int p = 0; p < PIECE_TYPE_NB; ++p) {
            occupancy[c] |= pieces[c][p];
        }
        allOccupancy |= occupancy[c];
    }
}

// =========================
// Inizializza posizione di partenza
// =========================
void Board::initStartPosition() {
    // Bianco
    pieces[WHITE][PAWN] = 0x000000000000FF00ULL;
    pieces[WHITE][ROOK] = 0x0000000000000081ULL;
    pieces[WHITE][KNIGHT] = 0x0000000000000042ULL;
    pieces[WHITE][BISHOP] = 0x0000000000000024ULL;
    pieces[WHITE][QUEEN] = 0x0000000000000008ULL;
    pieces[WHITE][KING] = 0x0000000000000010ULL;

    // Nero (stessa disposizione speculare)
    pieces[BLACK][PAWN] = 0x00FF000000000000ULL;
    pieces[BLACK][ROOK] = 0x8100000000000000ULL;
    pieces[BLACK][KNIGHT] = 0x4200000000000000ULL;
    pieces[BLACK][BISHOP] = 0x2400000000000000ULL;
    pieces[BLACK][QUEEN] = 0x0800000000000000ULL;
    pieces[BLACK][KING] = 0x1000000000000000ULL;

    // Diritti arrocco
    castlingRights[WHITE][0] = true; // Re-side
    castlingRights[WHITE][1] = true; // Q-side
    castlingRights[BLACK][0] = true;
    castlingRights[BLACK][1] = true;

    // Reset altre info
    enPassantSquare = -1;
    halfmoveClock = 0;
    fullmoveNumber = 1;
    sideToMove = WHITE;

    updateOccupancy();
}

// =========================
// Stampa la scacchiera in ASCII
// =========================
void Board::printBoard() const {
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " ";
        for (int file = 0; file < 8; ++file) {
            Square sq = rank * 8 + file;
            char pieceChar = '.';

            // Controlla ogni colore e tipo pezzo
            for (int c = 0; c < COLOR_NB; ++c) {
                for (int p = 0; p < PIECE_TYPE_NB; ++p) {
                    if (pieces[c][p] & (1ULL << sq)) {
                        switch (p) {
                        case PAWN:   pieceChar = 'P'; break;
                        case KNIGHT: pieceChar = 'N'; break;
                        case BISHOP: pieceChar = 'B'; break;
                        case ROOK:   pieceChar = 'R'; break;
                        case QUEEN:  pieceChar = 'Q'; break;
                        case KING:   pieceChar = 'K'; break;
                        }
                        if (c == BLACK)
                            pieceChar = tolower(pieceChar);
                    }
                }
            }
            std::cout << pieceChar << " ";
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
    std::cout << "Side to move: " << (sideToMove == WHITE ? "WHITE" : "BLACK") << "\n";
}

PieceType Board::pieceOn(Square sq) const
{
    Bitboard mask = 1ULL << sq;

    for (int pt = PAWN; pt < PIECE_TYPE_NB; pt++)
    {
        if (pieces[WHITE][pt] & mask)
            return PieceType(pt);

        if (pieces[BLACK][pt] & mask)
            return PieceType(pt);
    }

    return PIECE_TYPE_NB;
}

bool Board::pieceOn(Square sq, Color& color, PieceType& pt) const
{
    Bitboard mask = 1ULL << sq;

    for (int c = WHITE; c <= BLACK; c++)
    {
        for (int p = PAWN; p < PIECE_TYPE_NB; p++)
        {
            if (pieces[c][p] & mask)
            {
                color = Color(c);
                pt = PieceType(p);
                return true;
            }
        }
    }

    return false;
}