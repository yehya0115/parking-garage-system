#include "Ticket.h"

#include <limits>
#include <stdexcept>

int Ticket::lastNumber = 0;

Ticket::Ticket(int floor, int row, int column,
               const std::string& plate, const Time& entryTime)
    : number(0),
      floor(floor),
      row(row),
      column(column),
      plate(plate),
      entryTime(entryTime)
{
    if (floor < 0 || row < 0 || column < 0)
    {
        throw std::invalid_argument(
            "Ticket coordinates cannot be negative.");
    }

    if (plate.find_first_not_of(" \t\n\r") == std::string::npos)
    {
        throw std::invalid_argument("License plate cannot be empty.");
    }

    if (lastNumber == std::numeric_limits<int>::max())
    {
        throw std::overflow_error("Ticket numbers are exhausted.");
    }

    number = ++lastNumber;
}

int Ticket::getNumber() const
{
    return number;
}

int Ticket::getFloor() const
{
    return floor;
}

int Ticket::getRow() const
{
    return row;
}

int Ticket::getColumn() const
{
    return column;
}

const std::string& Ticket::getPlate() const
{
    return plate;
}

const Time& Ticket::getEntryTime() const
{
    return entryTime;
}