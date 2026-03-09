#include "attacks.h"
//aggiunti per debug, poi da togliere
#include <iostream>
#include <bitset>

namespace Attacks {

    Bitboard knightAttacks[64];
    Bitboard kingAttacks[64];

    static const Bitboard FILE_A = 0x0101010101010101ULL;
    static const Bitboard FILE_B = 0x0202020202020202ULL;
    static const Bitboard FILE_G = 0x4040404040404040ULL;
    static const Bitboard FILE_H = 0x8080808080808080ULL;

    void init()
    {
        for (int sq = 0; sq < 64; sq++)
        {
            //INIZIALIZZO CAVALLO
            Bitboard b = 1ULL << sq;
            Bitboard attacks = 0ULL;

            // 2 su + 1 dx
            attacks |= (b & ~FILE_H) << 17;

            // 2 su + 1 sx
            attacks |= (b & ~FILE_A) << 15;

            // 2 dx + 1 su
            attacks |= (b & ~(FILE_G | FILE_H)) << 10;

            // 2 sx + 1 su
            attacks |= (b & ~(FILE_A | FILE_B)) << 6;

            // 2 giù + 1 dx
            attacks |= (b & ~FILE_H) >> 15;

            // 2 giù + 1 sx
            attacks |= (b & ~FILE_A) >> 17;

            // 2 dx + 1 giù
            attacks |= (b & ~(FILE_G | FILE_H)) >> 6;

            // 2 sx + 1 giù
            attacks |= (b & ~(FILE_A | FILE_B)) >> 10;

            knightAttacks[sq] = attacks;
        }
        //INIZIALIZZO RE
        for (int sq = 0; sq < 64; sq++)
        {
            Bitboard attacks = 0ULL;

            int rank = sq / 8;
            int file = sq % 8;

            for (int dr = -1; dr <= 1; dr++)
            {
                for (int df = -1; df <= 1; df++)
                {
                    if (dr == 0 && df == 0)
                        continue;

                    int r = rank + dr;
                    int f = file + df;

                    if (r >= 0 && r <= 7 && f >= 0 && f <= 7)
                    {
                        attacks |= (1ULL << (r * 8 + f));
                    }
                }
            }

            kingAttacks[sq] = attacks;
        }
    }
    // ==========================
    // Bishop attacks
    // ==========================
    Bitboard getBishopAttacks(Square sq, Bitboard occupancy)
    {
        Bitboard attacks = 0ULL;

        int rank = sq / 8;
        int file = sq % 8;

        // NE
        for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++)
        {
            int s = r * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // NW
        for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--)
        {
            int s = r * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // SE
        for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++)
        {
            int s = r * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // SW
        for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
        {
            int s = r * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        return attacks;
    }


    // ==========================
    // Rook attacks
    // ==========================
    Bitboard getRookAttacks(Square sq, Bitboard occupancy)
    {
        Bitboard attacks = 0ULL;

        int rank = sq / 8;
        int file = sq % 8;

        // Nord
        for (int r = rank + 1; r <= 7; r++)
        {
            int s = r * 8 + file;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // Sud
        for (int r = rank - 1; r >= 0; r--)
        {
            int s = r * 8 + file;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // Est
        for (int f = file + 1; f <= 7; f++)
        {
            int s = rank * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        // Ovest
        for (int f = file - 1; f >= 0; f--)
        {
            int s = rank * 8 + f;
            attacks |= (1ULL << s);
            if (occupancy & (1ULL << s))
                break;
        }

        return attacks;
    }
}