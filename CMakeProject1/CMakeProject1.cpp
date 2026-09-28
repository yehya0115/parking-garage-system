#include <iostream>
#include <memory>
#include <stdexcept>

#include "Garage.h"
#include "Vehicles.h"

void check(bool condition, const char* message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }

    std::cout << "PASS: " << message << '\n';
}

int main()
{
    try
    {
        Garage garage(2, 1, 3);

        check(garage.getFloorCount() == 2, "Garage has two floors");
        check(garage.getOccupiedCount() == 0, "Garage starts empty");

        const Time entry(0, 23, 30);

        std::unique_ptr<Vehicle> car =
            std::make_unique<Car>("CAR-123");

        Ticket carTicket = garage.parkVehicle(car, entry);

        check(!car, "Car ownership moved into garage");
        check(
            carTicket.getFloor() == 0 &&
            carTicket.getColumn() == 1,
            "First car uses Medium slot on floor 0"
        );

        std::unique_ptr<Vehicle> secondCar =
            std::make_unique<Car>("CAR-456");

        Ticket secondTicket = garage.parkVehicle(secondCar, entry);

        check(
            secondTicket.getFloor() == 1 &&
            secondTicket.getColumn() == 1,
            "Second car chooses Medium upstairs before Large downstairs"
        );

        std::unique_ptr<Vehicle> bike =
            std::make_unique<Motorcycle>("BIKE-100");

        Ticket bikeTicket = garage.parkVehicle(bike, entry);

        check(
            bikeTicket.getFloor() == 0 &&
            bikeTicket.getColumn() == 0,
            "Motorcycle uses Small slot"
        );

        check(garage.getOccupiedCount() == 3, "Three slots occupied");

        check(
            garage.calculateFee(carTicket.getNumber(), entry) == Money(0),
            "Zero minutes costs zero"
        );

        check(
            garage.calculateFee(
                carTicket.getNumber(), Time(0, 23, 31)
            ) == Money(2000),
            "One minute costs one full hour"
        );

        check(
            garage.calculateFee(
                carTicket.getNumber(), Time(1, 0, 30)
            ) == Money(2000),
            "Exactly 60 minutes costs one hour"
        );

        const Money fee = garage.calculateFee(
            carTicket.getNumber(), Time(1, 1, 0)
        );

        check(fee == Money(4000), "90 minutes overnight costs two hours");
        std::cout << "Car fee: " << fee << "\n\n";

        std::unique_ptr<Vehicle> duplicate =
            std::make_unique<Car>("CAR-123");

        try
        {
            garage.parkVehicle(duplicate, entry);
            throw std::runtime_error("Duplicate plate was accepted");
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        check(
            duplicate != nullptr && garage.getOccupiedCount() == 3,
            "Duplicate rejection preserves ownership and occupancy"
        );

        try
        {
            garage.retrieveVehicle(
                carTicket.getNumber(), Time(0, 23, 0)
            );
            throw std::runtime_error("Earlier exit time was accepted");
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        check(
            garage.getOccupiedCount() == 3 &&
            garage.getTicket(carTicket.getNumber()).getPlate() == "CAR-123",
            "Invalid exit leaves vehicle and ticket active"
        );

        std::unique_ptr<Vehicle> returnedCar = garage.retrieveVehicle(
            carTicket.getNumber(), Time(1, 1, 0)
        );

        check(
            returnedCar && returnedCar->getPlate() == "CAR-123",
            "Retrieval returns ownership of the correct car"
        );

        check(
            garage.getOccupiedCount() == 2,
            "Retrieval frees the parking slot"
        );

        try
        {
            garage.retrieveVehicle(
                carTicket.getNumber(), Time(1, 1, 0)
            );
            throw std::runtime_error("Used ticket was accepted");
        }
        catch (const InvalidTicketException& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        std::unique_ptr<Vehicle> truck1 =
            std::make_unique<Truck>("TRUCK-1");

        std::unique_ptr<Vehicle> truck2 =
            std::make_unique<Truck>("TRUCK-2");

        garage.parkVehicle(truck1, entry);
        garage.parkVehicle(truck2, entry);

        std::unique_ptr<Vehicle> truck3 =
            std::make_unique<Truck>("TRUCK-3");

        try
        {
            garage.parkVehicle(truck3, entry);
            throw std::runtime_error("Truck parked without a suitable slot");
        }
        catch (const GarageFullException& error)
        {
            std::cout << "Expected error: " << error.what() << '\n';
        }

        check(
            truck3 != nullptr && garage.getOccupiedCount() == 4,
            "No suitable slot preserves truck ownership and occupancy"
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

        std::cout << "\nAll garage checks passed.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << "TEST FAILED: " << error.what() << '\n';
        return 1;
    }

    return 0;
}