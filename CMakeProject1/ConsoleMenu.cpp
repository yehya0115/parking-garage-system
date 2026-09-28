#include "ConsoleMenu.h"
#include "Vehicles.h"

#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
    std::string readLine(const std::string& prompt)
    {
        std::cout << prompt;

        std::string line;

        if (!std::getline(std::cin, line))
        {
            throw std::runtime_error("Input stream closed.");
        }

        return line;
    }

    int readInt(const std::string& prompt, int minimum, int maximum)
    {
        while (true)
        {
            const std::string line = readLine(prompt);
            std::istringstream input(line);

            int value = 0;
            char extra = '\0';

            if ((input >> value) &&
                !(input >> extra) &&
                value >= minimum &&
                value <= maximum)
            {
                return value;
            }

            std::cout << "Enter a whole number between "
                      << minimum << " and " << maximum << ".\n";
        }
    }

    Time readTime(const std::string& label)
    {
        std::cout << "\n" << label << '\n';

        const int day = readInt(
            "Day (starts at 0): ",
            0,
            std::numeric_limits<int>::max() / 1440
        );

        const int hour = readInt("Hour (0-23): ", 0, 23);
        const int minute = readInt("Minute (0-59): ", 0, 59);

        return Time(day, hour, minute);
    }

    std::string readPlate()
    {
        while (true)
        {
            std::string plate = readLine("License plate: ");

            const auto first = plate.find_first_not_of(" \t\r\n");

            if (first == std::string::npos)
            {
                std::cout << "License plate cannot be empty.\n";
                continue;
            }

            const auto last = plate.find_last_not_of(" \t\r\n");

            return plate.substr(first, last - first + 1);
        }
    }

    void printTicket(const Ticket& ticket)
    {
        std::cout << "\nTicket number: " << ticket.getNumber()
                  << "\nPlate: " << ticket.getPlate()
                  << "\nEntry: " << ticket.getEntryTime()
                  << "\nFloor: " << ticket.getFloor()
                  << "\nRow: " << ticket.getRow()
                  << "\nColumn: " << ticket.getColumn()
                  << '\n';
    }

    void parkVehicle(Garage& garage)
    {
        std::cout << "\n1. Motorcycle - 10 EGP/hour"
                  << "\n2. Car        - 20 EGP/hour"
                  << "\n3. Truck      - 40 EGP/hour\n";

        const int type = readInt("Vehicle type: ", 1, 3);
        const std::string plate = readPlate();

        std::unique_ptr<Vehicle> vehicle;

        switch (type)
        {
        case 1:
            vehicle = std::make_unique<Motorcycle>(plate);
            break;

        case 2:
            vehicle = std::make_unique<Car>(plate);
            break;

        case 3:
            vehicle = std::make_unique<Truck>(plate);
            break;
        }

        const Time entry = readTime("Entry time");
        const Ticket ticket = garage.parkVehicle(vehicle, entry);

        std::cout << "\nVehicle parked successfully.\n";
        printTicket(ticket);
    }

    int readTicketNumber()
    {
        return readInt(
            "Ticket number: ",
            1,
            std::numeric_limits<int>::max()
        );
    }

    void showFee(const Garage& garage)
    {
        const int number = readTicketNumber();

        printTicket(garage.getTicket(number));

        const Time exit = readTime("Proposed exit time");
        const Money fee = garage.calculateFee(number, exit);

        std::cout << "\nParking fee: " << fee << '\n';
    }

    void retrieveVehicle(Garage& garage)
    {
        const int number = readTicketNumber();

        printTicket(garage.getTicket(number));

        const Time exit = readTime("Exit time");
        const Money fee = garage.calculateFee(number, exit);

        std::cout << "\nParking fee: " << fee << '\n';

        const int confirm = readInt(
            "Retrieve vehicle? (1 = Yes, 0 = Cancel): ", 0, 1
        );

        if (confirm == 0)
        {
            std::cout << "Retrieval cancelled. Vehicle remains parked.\n";
            return;
        }

        std::unique_ptr<Vehicle> vehicle =
            garage.retrieveVehicle(number, exit);

        std::cout << "Vehicle retrieved: " << vehicle->getPlate()
                  << "\nTicket is now closed.\n";
    }

    char sizeSymbol(SlotSize size)
    {
        switch (size)
        {
        case SlotSize::Small:
            return 'S';

        case SlotSize::Medium:
            return 'M';

        case SlotSize::Large:
            return 'L';
        }

        return '?';
    }

    void showMap(const Garage& garage)
    {
        std::cout << "\nS = Small, M = Medium, L = Large"
                  << "\nX = Occupied, . = Empty"
                  << "\nFloor, row, and column numbering starts at 0.\n";

        for (int index = 0; index < garage.getFloorCount(); ++index)
        {
            const Floor& floor = garage.getFloor(index);

            std::cout << "\nFloor " << floor.getNumber()
                      << " | Occupied: " << floor.getOccupiedCount()
                      << '/' << floor.getRows() * floor.getColumns()
                      << '\n';

            for (int row = 0; row < floor.getRows(); ++row)
            {
                std::cout << "Row " << row << ": ";

                for (int column = 0; column < floor.getColumns(); ++column)
                {
                    const ParkingSlot& slot = floor.getSlot(row, column);

                    std::cout << '[' << column << ':'
                              << sizeSymbol(slot.getSize())
                              << (slot.isOccupied() ? 'X' : '.')
                              << "] ";
                }

                std::cout << '\n';
            }
        }

        std::cout << "\nTotal occupied: "
                  << garage.getOccupiedCount() << '\n';
    }
}

void runConsoleMenu(Garage& garage)
{
    while (true)
    {
        try
        {
            std::cout << "\n===== Parking Garage System ====="
                      << "\n1. Park a vehicle"
                      << "\n2. Retrieve a vehicle"
                      << "\n3. Calculate parking fee"
                      << "\n4. Show occupancy map"
                      << "\n5. Show ticket details"
                      << "\n0. Exit\n";

            const int choice = readInt("Choose: ", 0, 5);

            switch (choice)
            {
            case 1:
                parkVehicle(garage);
                break;

            case 2:
                retrieveVehicle(garage);
                break;

            case 3:
                showFee(garage);
                break;

            case 4:
                showMap(garage);
                break;

            case 5:
                printTicket(garage.getTicket(readTicketNumber()));
                break;

            case 0:
                std::cout << "Goodbye!\n";
                return;
            }
        }
        catch (const std::exception& error)
        {
            if (!std::cin)
            {
                std::cout << "\nInput closed. Exiting.\n";
                return;
            }

            std::cout << "\nError: " << error.what() << '\n';
        }
    }
}