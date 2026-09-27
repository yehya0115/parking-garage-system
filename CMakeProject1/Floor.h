#pragma once

#include <vector>

#include "ParkingSlot.h"

class Floor
{
private:
    int number;
    int rows;
    int columns;
    std::vector<std::vector<ParkingSlot>> slots;

public:
    Floor(int number, int rows, int columns);

    Floor(const Floor&) = delete;
    Floor& operator=(const Floor&) = delete;

    Floor(Floor&&) noexcept = default;
    Floor& operator=(Floor&&) noexcept = default;

    int getNumber() const;
    int getRows() const;
    int getColumns() const;
    int getOccupiedCount() const;

    ParkingSlot& getSlot(int row, int column);
    const ParkingSlot& getSlot(int row, int column) const;
};