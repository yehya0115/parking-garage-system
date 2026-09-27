#include "ParkingSlot.h"

#include <stdexcept>
#include <utility>

ParkingSlot::ParkingSlot(SlotSize size) : size(size)
{
    if (size != SlotSize::Small &&
        size != SlotSize::Medium &&
        size != SlotSize::Large)
    {
        throw std::invalid_argument("Invalid slot size.");
    }
}

SlotSize ParkingSlot::getSize() const
{
    return size;
}

bool ParkingSlot::isOccupied() const
{
    return vehicle != nullptr;
}

bool ParkingSlot::canFit(const Vehicle& incomingVehicle) const
{
    return incomingVehicle.getRequiredSize() <= size;
}

const Vehicle* ParkingSlot::getVehicle() const
{
    return vehicle.get();
}

void ParkingSlot::park(std::unique_ptr<Vehicle>& incomingVehicle)
{
    if (!incomingVehicle)
    {
        throw std::invalid_argument("Cannot park a null vehicle.");
    }

    if (isOccupied())
    {
        throw std::runtime_error("Parking slot is already occupied.");
    }

    if (!canFit(*incomingVehicle))
    {
        throw std::invalid_argument("Vehicle is too large for this slot.");
    }

    vehicle = std::move(incomingVehicle);
}

std::unique_ptr<Vehicle> ParkingSlot::retrieve()
{
    if (!isOccupied())
    {
        throw std::runtime_error("Parking slot is empty.");
    }

    return std::move(vehicle);
}