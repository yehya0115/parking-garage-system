#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Floor.h"
#include "Ticket.h"
#include "GarageExceptions.h"

class Garage
{
private:
    std::vector<Floor> floors;
    std::map<int, Ticket> activeTickets;

public:
    Garage(int floorCount, int rows, int columns);

    Garage(const Garage&) = delete;
    Garage& operator=(const Garage&) = delete;

    Garage(Garage&&) noexcept = default;
    Garage& operator=(Garage&&) noexcept = default;

    Ticket parkVehicle(
        std::unique_ptr<Vehicle>& vehicle,
        const Time& entryTime
    );

    Money calculateFee(
        int ticketNumber,
        const Time& exitTime
    ) const;

    std::unique_ptr<Vehicle> retrieveVehicle(
        int ticketNumber,
        const Time& exitTime
    );

    const Ticket& getTicket(int ticketNumber) const;

    const Floor& getFloor(int index) const;

    int getFloorCount() const;
    int getOccupiedCount() const;
};