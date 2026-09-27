#include "Vehicles.h"

Car::Car(const std::string& plate) : Vehicle(plate)
{
}

std::string Car::getType() const
{
    return "Car";
}

Money Car::getHourlyRate() const
{
    return Money(2000);
}

SlotSize Car::getRequiredSize() const
{
    return SlotSize::Medium;
}

Motorcycle::Motorcycle(const std::string& plate) : Vehicle(plate)
{
}

std::string Motorcycle::getType() const
{
    return "Motorcycle";
}

Money Motorcycle::getHourlyRate() const
{
    return Money(1000);
}

SlotSize Motorcycle::getRequiredSize() const
{
    return SlotSize::Small;
}

Truck::Truck(const std::string& plate) : Vehicle(plate)
{
}

std::string Truck::getType() const
{
    return "Truck";
}

Money Truck::getHourlyRate() const
{
    return Money(4000);
}

SlotSize Truck::getRequiredSize() const
{
    return SlotSize::Large;
}