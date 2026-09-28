#pragma once

#include <string>

#include "Time.h"

class Ticket
{
private:
    static int lastNumber;

    int number;
    int floor;
    int row;
    int column;
    std::string plate;
    Time entryTime;

public:
    Ticket(int floor, int row, int column,
           const std::string& plate, const Time& entryTime);

    int getNumber() const;
    int getFloor() const;
    int getRow() const;
    int getColumn() const;

    const std::string& getPlate() const;
    const Time& getEntryTime() const;
};