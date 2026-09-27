#include <iostream>
#include <memory>
#include <stdexcept>

#include "ParkingSlot.h"
#include "Vehicles.h"

int main()
{
    try
    {
        std::cout << std::boolalpha;

        ParkingSlot slot(SlotSize::Medium);
        std::unique_ptr<Vehicle> car =
            std::make_unique<Car>("CAR-123");

        std::cout << "Before parking:\n";
        std::cout << "Caller owns car: " << (car != nullptr) << '\n';
        std::cout << "Slot occupied: " << slot.isOccupied() << "\n\n";

        slot.park(car);

        std::cout << "After parking:\n";
        std::cout << "Caller pointer is null: " << (car == nullptr) << '\n';
        std::cout << "Slot occupied: " << slot.isOccupied() << '\n';
        std::cout << "Parked plate: "
            << slot.getVehicle()->getPlate() << "\n\n";

        std::unique_ptr<Vehicle> bike =
            std::make_unique<Motorcycle>("BIKE-456");

        try
        {
            slot.park(bike);
            std::cerr << "Test failed: occupied slot accepted a vehicle.\n";
            return 1;
        }
        catch (const std::runtime_error& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        std::cout << "Caller still owns bike: "
            << (bike != nullptr) << "\n\n";

        car = slot.retrieve();

        std::cout << "After retrieval:\n";
        std::cout << "Caller owns car: " << (car != nullptr) << '\n';
        std::cout << "Slot occupied: " << slot.isOccupied() << "\n\n";

        std::unique_ptr<Vehicle> truck =
            std::make_unique<Truck>("TRUCK-789");

        try
        {
            slot.park(truck);
            std::cerr << "Test failed: oversized vehicle was accepted.\n";
            return 1;
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        std::cout << "Caller still owns truck: "
            << (truck != nullptr) << '\n';

        slot.park(bike);

        std::cout << "Motorcycle fits in Medium slot: "
            << slot.isOccupied() << '\n';

        bike = slot.retrieve();

        try
        {
            auto missingVehicle = slot.retrieve();
            std::cerr << "Test failed: empty slot retrieval succeeded.\n";
            return 1;
        }
        catch (const std::runtime_error& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        std::unique_ptr<Vehicle> emptyVehicle;

        try
        {
            slot.park(emptyVehicle);
            std::cerr << "Test failed: null vehicle was accepted.\n";
            return 1;
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Unexpected error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}