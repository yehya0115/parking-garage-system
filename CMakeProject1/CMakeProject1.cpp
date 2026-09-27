#include <iostream>
#include <memory>
#include <stdexcept>

#include "Floor.h"
#include "Vehicles.h"

int main()
{
    try
    {
        Floor floor(0, 2, 3);

        std::cout << "Floor: " << floor.getNumber() << '\n';
        std::cout << "Rows: " << floor.getRows() << '\n';
        std::cout << "Columns: " << floor.getColumns() << '\n';
        std::cout << "Initially occupied: "
            << floor.getOccupiedCount() << '\n';

        std::unique_ptr<Vehicle> bike =
            std::make_unique<Motorcycle>("BIKE-100");

        std::unique_ptr<Vehicle> car =
            std::make_unique<Car>("CAR-200");

        std::unique_ptr<Vehicle> truck =
            std::make_unique<Truck>("TRUCK-300");

        floor.getSlot(0, 0).park(bike);
        floor.getSlot(0, 1).park(car);
        floor.getSlot(1, 2).park(truck);

        std::cout << "After parking: "
            << floor.getOccupiedCount() << '\n';

        const Floor& view = floor;

        std::cout << "\nOccupancy map (X = occupied, . = empty):\n";

        for (int row = 0; row < view.getRows(); ++row)
        {
            for (int column = 0; column < view.getColumns(); ++column)
            {
                const ParkingSlot& slot = view.getSlot(row, column);
                std::cout << (slot.isOccupied() ? 'X' : '.') << ' ';
            }

            std::cout << '\n';
        }

        car = floor.getSlot(0, 1).retrieve();

        std::cout << "\nRetrieved plate: " << car->getPlate() << '\n';
        std::cout << "After retrieval: "
            << floor.getOccupiedCount() << '\n';

        try
        {
            floor.getSlot(2, 0);
            std::cerr << "Test failed: invalid coordinates accepted.\n";
            return 1;
        }
        catch (const std::out_of_range& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        try
        {
            Floor invalidFloor(0, 0, 3);
            std::cerr << "Test failed: zero rows accepted.\n";
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