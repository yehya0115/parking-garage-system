#pragma once

#include <ostream>

class Time
{
private:
    int totalMinutes;

public:
    Time(int day = 0, int hour = 0, int minute = 0);

    int operator-(const Time& other) const;

    bool operator==(const Time& other) const;
    bool operator<(const Time& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Time& time);
};