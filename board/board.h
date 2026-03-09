#pragma once

#include "../core/type.h"
#include "../core/undo.h"
#include <vector>

class Board {
public:
    // =========================
    // Bitboard dei pezzi [color][pieceType]
    // =========================
    Bitboard pieces[COLOR_NB][PIECE_TYPE_NB];  // [2][6]

    // Bitboard occupazione
    Bitboard occupancy[COLOR_NB];  // WHITE, BLACK
    Bitboard allOccupancy;         // Tutti i pezzi

    // Lato che deve muovere
    Color sideToMove;

    // Diritti arrocco
    bool castlingRights[COLOR_NB][2]; // [WHITE/BLACK][K/Q-side]

    // En passant (casella target se disponibile, altrimenti -1)
    Square enPassantSquare;

    // Halfmove clock per regola 50 mosse
    int halfmoveClock;

    // Numero mossa (incrementa dopo il nero)
    int fullmoveNumber;

    std::vector<UndoState> history;

public:
    // =========================
    // Costruttore
    // =========================
    Board();

    // =========================
    // Inizializza posizione di partenza
    // =========================
    void initStartPosition();

    // =========================
    // Aggiorna bitboard di occupazione
    // =========================
    void updateOccupancy();

    // =========================
    // Stampa la scacchiera (per debug)
    // =========================
    void printBoard() const;

    
    //per trovare il pezzo su quella casella
    PieceType pieceOn(Square sq) const;

    //se voglio anche il colore del pezzo che è su quella casa
    bool pieceOn(Square sq, Color& color, PieceType& pt) const;
};

