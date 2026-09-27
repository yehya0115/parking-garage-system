#include "Vehicle.h"

#include <stdexcept>

Vehicle::Vehicle(const std::string& plate) : plate(plate)
{
    if (plate.find_first_not_of(" \t\n\r") == std::string::npos)
    {
        throw std::invalid_argument("License plate cannot be empty.");
    }
}

const std::string& Vehicle::getPlate() const
{
    return plate;
}