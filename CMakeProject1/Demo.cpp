#include "Demo.h"
#include "Garage.h"
#include "Vehicles.h"

#include <iostream>
#include <memory>
#include <stdexcept>

namespace
{
    void check(bool condition, const char* message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }

        std::cout << "PASS: " << message << '\n';
    }

    void printMap(const Garage& garage)
    {
        std::cout << "\nOccupancy map (X = occupied, . = empty)\n";

        for (int index = 0; index < garage.getFloorCount(); ++index)
        {
            const Floor& floor = garage.getFloor(index);

            std::cout << "Floor " << floor.getNumber() << '\n';

            for (int row = 0; row < floor.getRows(); ++row)
            {
                for (int column = 0; column < floor.getColumns(); ++column)
                {
                    std::cout
                        << (floor.getSlot(row, column).isOccupied()
                            ? 'X' : '.')
                        << ' ';
                }

                std::cout << '\n';
            }
        }

        std::cout << "Total occupied: "
                  << garage.getOccupiedCount() << "\n\n";
    }
}

void runDemo()
{
    std::cout << "===== Automatic Garage Demo =====\n";
    std::cout << std::boolalpha;

    Garage garage(2, 1, 3);

    const Time entry(0, 23, 30);
    const Time exit(1, 1, 0);

    check(garage.getOccupiedCount() == 0, "Garage starts empty");

    std::unique_ptr<Vehicle> car =
        std::make_unique<Car>("DEMO-CAR-1");

    std::cout << "\nBefore parking, caller owns car: "
              << (car != nullptr) << '\n';

    const Ticket carTicket = garage.parkVehicle(car, entry);

    check(!car, "Parking transfers car ownership into the slot");

    check(
        carTicket.getFloor() == 0 &&
        carTicket.getColumn() == 1,
        "Car uses a Medium slot on floor 0"
    );

    const Ticket ticketCopy = carTicket;

    check(
        ticketCopy.getNumber() == carTicket.getNumber(),
        "Copying a ticket preserves its number"
    );

    std::unique_ptr<Vehicle> secondCar =
        std::make_unique<Car>("DEMO-CAR-2");

    const Ticket secondTicket = garage.parkVehicle(secondCar, entry);

    check(
        secondTicket.getFloor() == 1 &&
        secondTicket.getColumn() == 1,
        "Smallest suitable slot is chosen across floors"
    );

    check(
        secondTicket.getNumber() != carTicket.getNumber(),
        "New tickets have different numbers"
    );

    std::unique_ptr<Vehicle> bike =
        std::make_unique<Motorcycle>("DEMO-BIKE");

    const Ticket bikeTicket = garage.parkVehicle(bike, entry);

    check(
        bikeTicket.getColumn() == 0,
        "Motorcycle uses a Small slot"
    );

    std::unique_ptr<Vehicle> truck =
        std::make_unique<Truck>("DEMO-TRUCK");

    const Ticket truckTicket = garage.parkVehicle(truck, entry);

    check(
        truckTicket.getColumn() == 2,
        "Truck uses a Large slot"
    );

    printMap(garage);

    check(exit - entry == 90, "Overnight duration is 90 minutes");

    const Money carFee = garage.calculateFee(carTicket.getNumber(), exit);
    const Money bikeFee = garage.calculateFee(bikeTicket.getNumber(), exit);
    const Money truckFee = garage.calculateFee(truckTicket.getNumber(), exit);

    std::cout << "\nCar fee: " << carFee
              << "\nMotorcycle fee: " << bikeFee
              << "\nTruck fee: " << truckFee << '\n';

    check(carFee == Money(4000), "Car fee is 40 EGP");
    check(bikeFee == Money(2000), "Motorcycle fee is 20 EGP");
    check(truckFee == Money(8000), "Truck fee is 80 EGP");

    check(
        garage.calculateFee(carTicket.getNumber(), entry) == Money(0),
        "Zero duration is free"
    );

    check(
        garage.calculateFee(
            carTicket.getNumber(), Time(0, 23, 31)
        ) == Money(2000),
        "One minute is charged as one hour"
    );

    std::unique_ptr<Vehicle> duplicate =
        std::make_unique<Car>("DEMO-CAR-1");

    try
    {
        garage.parkVehicle(duplicate, entry);
        throw std::runtime_error("Duplicate plate was accepted");
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "\nExpected error: " << error.what() << '\n';
    }

    check(
        duplicate != nullptr && garage.getOccupiedCount() == 4,
        "Rejected duplicate keeps caller ownership"
    );

    try
    {
        garage.retrieveVehicle(carTicket.getNumber(), Time(0, 22, 0));
        throw std::runtime_error("Earlier exit was accepted");
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Expected error: " << error.what() << '\n';
    }

    check(
        garage.getOccupiedCount() == 4,
        "Invalid exit leaves occupancy unchanged"
    );

    std::unique_ptr<Vehicle> returnedCar =
        garage.retrieveVehicle(carTicket.getNumber(), exit);

    check(
        returnedCar && returnedCar->getPlate() == "DEMO-CAR-1",
        "Retrieval transfers the correct car back to the caller"
    );

    check(
        garage.getOccupiedCount() == 3,
        "Retrieval frees a slot"
    );

    try
    {
        garage.getTicket(carTicket.getNumber());
        throw std::runtime_error("Used ticket remained active");
    }
    catch (const InvalidTicketException& error)
    {
        std::cout << "Expected error: " << error.what() << '\n';
    }

    std::unique_ptr<Vehicle> secondTruck =
        std::make_unique<Truck>("DEMO-TRUCK-2");

    garage.parkVehicle(secondTruck, entry);

    std::unique_ptr<Vehicle> thirdTruck =
        std::make_unique<Truck>("DEMO-TRUCK-3");

    try
    {
        garage.parkVehicle(thirdTruck, entry);
        throw std::runtime_error("Truck parked without a suitable slot");
    }
    catch (const GarageFullException& error)
    {
        std::cout << "Expected error: " << error.what() << '\n';
    }

    check(
        thirdTruck != nullptr && garage.getOccupiedCount() == 4,
        "Failed parking preserves ownership and occupancy"
    );

    std::unique_ptr<Vehicle> emptyVehicle;

    try
    {
        garage.parkVehicle(emptyVehicle, entry);
        throw std::runtime_error("Null vehicle was accepted");
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Expected error: " << error.what() << '\n';
    }

    printMap(garage);

    std::cout << "All demo checks passed.\n";
}