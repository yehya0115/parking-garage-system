#include "Floor.h"

#include <limits>
#include <stdexcept>

Floor::Floor(int number, int rows, int columns)
    : number(number), rows(rows), columns(columns)
{
    if (number < 0)
    {
        throw std::invalid_argument("Floor number cannot be negative.");
    }

    if (rows <= 0 || columns <= 0)
    {
        throw std::invalid_argument("Rows and columns must be positive.");
    }

    if (rows > std::numeric_limits<int>::max() / columns)
    {
        throw std::overflow_error("Too many parking slots.");
    }

    slots.resize(rows);

    for (auto& row : slots)
    {
        row.reserve(columns);

        for (int column = 0; column < columns; ++column)
        {
            SlotSize size;

            switch (column % 3)
            {
            case 0:
                size = SlotSize::Small;
                break;

            case 1:
                size = SlotSize::Medium;
                break;

            default:
                size = SlotSize::Large;
                break;
            }

            row.emplace_back(size);
        }
    }
}

int Floor::getNumber() const
{
    return number;
}

int Floor::getRows() const
{
    return rows;
}

int Floor::getColumns() const
{
    return columns;
}

int Floor::getOccupiedCount() const
{
    int count = 0;

    for (const auto& row : slots)
    {
        for (const auto& slot : row)
        {
            if (slot.isOccupied())
            {
                ++count;
            }
        }
    }

    return count;
}

ParkingSlot& Floor::getSlot(int row, int column)
{
    if (row < 0 || row >= rows || column < 0 || column >= columns)
    {
        throw std::out_of_range("Slot coordinates are out of range.");
    }

    return slots[row][column];
}

const ParkingSlot& Floor::getSlot(int row, int column) const
{
    if (row < 0 || row >= rows || column < 0 || column >= columns)
    {
        throw std::out_of_range("Slot coordinates are out of range.");
    }

    return slots[row][column];
}