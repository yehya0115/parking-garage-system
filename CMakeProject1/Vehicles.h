#pragma once

#include "Vehicle.h"

class Car : public Vehicle
{
public:
    explicit Car(const std::string& plate);

    std::string getType() const override;
    Money getHourlyRate() const override;
    SlotSize getRequiredSize() const override;
};

class Motorcycle : public Vehicle
{
public:
    explicit Motorcycle(const std::string& plate);

    std::string getType() const override;
    Money getHourlyRate() const override;
    SlotSize getRequiredSize() const override;
};

class Truck : public Vehicle
{
public:
    explicit Truck(const std::string& plate);

    std::string getType() const override;
    Money getHourlyRate() const override;
    SlotSize getRequiredSize() const override;
};