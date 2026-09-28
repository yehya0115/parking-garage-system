#include <exception>
#include <iostream>

#include "ConsoleMenu.h"
#include "Garage.h"

int main()
{
    try
    {
        Garage garage(2, 2, 3);

        runConsoleMenu(garage);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}