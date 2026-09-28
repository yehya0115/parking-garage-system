#include "Garage.h"

#include <limits>
#include <stdexcept>
#include <utility>

Garage::Garage(int floorCount, int rows, int columns)
{
    if (floorCount <= 0 || rows <= 0 || columns <= 0)
    {
        throw std::invalid_argument(
            "Floor count, rows, and columns must be positive."
        );
    }

    const int maxCount = std::numeric_limits<int>::max();

    if (rows > maxCount / columns)
    {
        throw std::overflow_error("Too many parking slots.");
    }

    const int slotsPerFloor = rows * columns;

    if (floorCount > maxCount / slotsPerFloor)
    {
        throw std::overflow_error("Too many parking slots.");
    }

    floors.reserve(floorCount);

    for (int index = 0; index < floorCount; ++index)
    {
        floors.emplace_back(index, rows, columns);
    }
}

Ticket Garage::parkVehicle(
    std::unique_ptr<Vehicle>& vehicle,
    const Time& entryTime)
{
    if (!vehicle)
    {
        throw std::invalid_argument("Cannot park a null vehicle.");
    }

    for (const auto& item : activeTickets)
    {
        if (item.second.getPlate() == vehicle->getPlate())
        {
            throw std::invalid_argument(
                "This license plate is already inside the garage."
            );
        }
    }

    ParkingSlot* bestSlot = nullptr;
    int bestFloor = -1;
    int bestRow = -1;
    int bestColumn = -1;

    for (auto& floor : floors)
    {
        for (int row = 0; row < floor.getRows(); ++row)
        {
            for (int column = 0; column < floor.getColumns(); ++column)
            {
                ParkingSlot& slot = floor.getSlot(row, column);

                if (slot.isOccupied() || !slot.canFit(*vehicle))
                {
                    continue;
                }

                if (bestSlot == nullptr ||
                    slot.getSize() < bestSlot->getSize())
                {
                    bestSlot = &slot;
                    bestFloor = floor.getNumber();
                    bestRow = row;
                    bestColumn = column;
                }
            }
        }
    }

    if (bestSlot == nullptr)
    {
        throw GarageFullException(
            "No free suitable slot is available."
        );
    }

    Ticket ticket(
        bestFloor,
        bestRow,
        bestColumn,
        vehicle->getPlate(),
        entryTime
    );

    // Save the ticket before transferring vehicle ownership.
    const auto result = activeTickets.emplace(
        ticket.getNumber(), ticket
    );

    if (!result.second)
    {
        throw std::logic_error("Duplicate ticket number.");
    }

    try
    {
        bestSlot->park(vehicle);
    }
    catch (...)
    {
        activeTickets.erase(result.first);
        throw;
    }

    return ticket;
}

Money Garage::calculateFee(
    int ticketNumber,
    const Time& exitTime) const
{
    const Ticket& ticket = getTicket(ticketNumber);

    if (exitTime < ticket.getEntryTime())
    {
        throw std::invalid_argument(
            "Exit time cannot be earlier than entry time."
        );
    }

    const int minutes = exitTime - ticket.getEntryTime();

    // Round up without risking overflow from minutes + 59.
    const int hours = minutes / 60 + (minutes % 60 != 0);

    const ParkingSlot& slot = getFloor(ticket.getFloor()).getSlot(
        ticket.getRow(), ticket.getColumn()
    );

    const Vehicle* vehicle = slot.getVehicle();

    if (vehicle == nullptr)
    {
        throw std::logic_error(
            "The ticket refers to an empty parking slot."
        );
    }

    return vehicle->getHourlyRate() * hours;
}

std::unique_ptr<Vehicle> Garage::retrieveVehicle(
    int ticketNumber,
    const Time& exitTime)
{
    // Validate the ticket, exit time, and fee before changing ownership.
    calculateFee(ticketNumber, exitTime);

    const Ticket& ticket = getTicket(ticketNumber);

    ParkingSlot& slot = floors[ticket.getFloor()].getSlot(
        ticket.getRow(), ticket.getColumn()
    );

    std::unique_ptr<Vehicle> vehicle = slot.retrieve();

    activeTickets.erase(ticketNumber);

    return vehicle;
}

const Ticket& Garage::getTicket(int ticketNumber) const
{
    const auto found = activeTickets.find(ticketNumber);

    if (found == activeTickets.end())
    {
        throw InvalidTicketException(
            "Ticket number was not found or was already used."
        );
    }

    return found->second;
}

const Floor& Garage::getFloor(int index) const
{
    if (index < 0 || index >= getFloorCount())
    {
        throw std::out_of_range("Floor index is out of range.");
    }

    return floors[index];
}

int Garage::getFloorCount() const
{
    return static_cast<int>(floors.size());
}

int Garage::getOccupiedCount() const
{
    int count = 0;

    for (const auto& floor : floors)
    {
        count += floor.getOccupiedCount();
    }

    return count;
}