#pragma once

#include <memory>

#include "Vehicle.h"

class ParkingSlot
{
private:
    SlotSize size;
    std::unique_ptr<Vehicle> vehicle;

public:
    explicit ParkingSlot(SlotSize size);

    SlotSize getSize() const;
    bool isOccupied() const;
    bool canFit(const Vehicle& incomingVehicle) const;

    const Vehicle* getVehicle() const;

    void park(std::unique_ptr<Vehicle>& incomingVehicle);
    std::unique_ptr<Vehicle> retrieve();
};