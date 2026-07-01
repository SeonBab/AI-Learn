#include "GameEngine.h"

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsoleEncoding()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

int main()
{
    setupConsoleEncoding();

    GameEngine game;
    std::cout << game.getIntroText();

    std::string input;
    while (game.isRunning())
    {
        std::cout << "\n> ";

        if (!std::getline(std::cin, input))
        {
            break;
        }

        std::cout << game.processCommand(input);
    }

    return 0;
}
