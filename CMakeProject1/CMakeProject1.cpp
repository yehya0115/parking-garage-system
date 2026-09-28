#include <exception>
#include <iostream>
#include <string>

#include "ConsoleMenu.h"
#include "Demo.h"
#include "Garage.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 2 && std::string(argv[1]) == "--demo")
        {
            runDemo();
            return 0;
        }

        if (argc != 1)
        {
            std::cerr << "Usage: " << argv[0] << " [--demo]\n";
            return 1;
        }

        Garage garage(2, 2, 3);
        runConsoleMenu(garage);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}