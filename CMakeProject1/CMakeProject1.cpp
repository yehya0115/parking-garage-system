#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Vehicles.h"

const char* getSizeName(SlotSize size)
{
    switch (size)
    {
    case SlotSize::Small:
        return "Small";

    case SlotSize::Medium:
        return "Medium";

    case SlotSize::Large:
        return "Large";
    }

    return "Unknown";
}

int main()
{
    try
    {
        std::vector<std::unique_ptr<Vehicle>> vehicles;

        vehicles.push_back(std::make_unique<Car>("CAR-123"));
        vehicles.push_back(std::make_unique<Motorcycle>("BIKE-456"));
        vehicles.push_back(std::make_unique<Truck>("TRUCK-789"));

        for (const auto& vehicle : vehicles)
        {
            std::cout << "Type: " << vehicle->getType() << '\n';
            std::cout << "Plate: " << vehicle->getPlate() << '\n';
            std::cout << "Hourly rate: "
                << vehicle->getHourlyRate() << '\n';
            std::cout << "Required size: "
                << getSizeName(vehicle->getRequiredSize()) << '\n';
            std::cout << "Fee for 3 hours: "
                << vehicle->getHourlyRate() * 3 << "\n\n";
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Unexpected error: " << error.what() << '\n';
        return 1;
    }

    std::cout << "Testing an empty license plate...\n";

    try
    {
        Car invalidCar("");
        std::cout << "Test failed: empty plate was accepted.\n";
        return 1;
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Expected error: " << error.what() << '\n';
    }

    return 0;
}