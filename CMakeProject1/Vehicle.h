#pragma once

#include <string>

#include "Money.h"
#include "SlotSize.h"

class Vehicle
{
private:
    std::string plate;

public:
    explicit Vehicle(const std::string& plate);
    virtual ~Vehicle() = default;

    const std::string& getPlate() const;

    virtual std::string getType() const = 0;
    virtual Money getHourlyRate() const = 0;
    virtual SlotSize getRequiredSize() const = 0;
};