#include <iostream>
#include <stdexcept>

#include "Ticket.h"

int main()
{
    try
    {
        Ticket first(0, 0, 1, "CAR-123", Time(0, 10, 30));
        Ticket second(1, 0, 2, "TRUCK-456", Time(0, 11, 0));

        std::cout << "First ticket: " << first.getNumber() << '\n';
        std::cout << "Plate: " << first.getPlate() << '\n';
        std::cout << "Entry: " << first.getEntryTime() << '\n';

        std::cout << "Location: floor " << first.getFloor()
            << ", row " << first.getRow()
            << ", column " << first.getColumn() << '\n';

        std::cout << "\nSecond ticket: "
            << second.getNumber() << '\n';

        Ticket copy = first;

        std::cout << "Copied ticket number: "
            << copy.getNumber() << '\n';

        std::cout << std::boolalpha;
        std::cout << "Copy keeps the same number: "
            << (copy.getNumber() == first.getNumber()) << '\n';

        try
        {
            Ticket invalid(-1, 0, 0, "BAD-100", Time());
            std::cerr << "Test failed: negative coordinates accepted.\n";
            return 1;
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "\nExpected error: " << error.what() << '\n';
        }

        try
        {
            Ticket invalid(0, 0, 0, "   ", Time());
            std::cerr << "Test failed: blank plate accepted.\n";
            return 1;
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        Ticket third(0, 1, 0, "BIKE-789", Time(0, 12, 0));

        std::cout << "\nThird ticket: " << third.getNumber() << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << "Unexpected error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}