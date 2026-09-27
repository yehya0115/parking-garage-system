#include "Time.h"

#include <limits>
#include <stdexcept>

Time::Time(int day, int hour, int minute) : totalMinutes(0)
{
    if (day < 0)
    {
        throw std::invalid_argument("Day cannot be negative.");
    }

    if (hour < 0 || hour > 23)
    {
        throw std::invalid_argument("Hour must be between 0 and 23.");
    }

    if (minute < 0 || minute > 59)
    {
        throw std::invalid_argument("Minute must be between 0 and 59.");
    }

    long long minutes =
        static_cast<long long>(day) * 1440 + hour * 60 + minute;

    if (minutes > std::numeric_limits<int>::max())
    {
        throw std::overflow_error("Time value is too large.");
    }

    totalMinutes = static_cast<int>(minutes);
}

int Time::operator-(const Time& other) const
{
    return totalMinutes - other.totalMinutes;
}

bool Time::operator==(const Time& other) const
{
    return totalMinutes == other.totalMinutes;
}

bool Time::operator<(const Time& other) const
{
    return totalMinutes < other.totalMinutes;
}

std::ostream& operator<<(std::ostream& out, const Time& time)
{
    int day = time.totalMinutes / 1440;
    int remainingMinutes = time.totalMinutes % 1440;
    int hour = remainingMinutes / 60;
    int minute = remainingMinutes % 60;

    out << "Day " << day << ' ';

    if (hour < 10)
    {
        out << '0';
    }

    out << hour << ':';

    if (minute < 10)
    {
        out << '0';
    }

    out << minute;

    return out;
}