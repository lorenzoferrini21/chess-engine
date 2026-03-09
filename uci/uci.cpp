#include "uci.h"
#include "../board/board.h"
#include "../search/search.h"
#include "../move/makemove.h"
#include "../movegen/attacks.h"

#include <iostream>
#include <sstream>
#include <string>

void UCI::loop()
{
    Board board;
    Attacks::init();
    board.initStartPosition();

    std::string line;

    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "uci")
        {
            std::cout << "id name MyEngine\n";
            std::cout << "id author You\n";
            std::cout << "uciok\n";
        }

        else if (token == "isready")
        {
            std::cout << "readyok\n";
        }

        else if (token == "ucinewgame")
        {
            board.initStartPosition();
        }

        else if (token == "position")
        {
            std::string type;
            iss >> type;

            if (type == "startpos")
            {
                board.initStartPosition();
            }
        }

        else if (token == "go")
        {
            //---------------------------------------------------------
            // questo parametro determina quando a fondo scenderenella 
            //ricerca e quindi  determina la forza del motore. Per ora 7 è il massimo
            //---------------------------------------------------------
            Move best = Search::findBestMove(board, 7); 
            std::cout << "bestmove " << best << std::endl;
        }

        else if (token == "quit")
        {
            break;
        }
    }
}