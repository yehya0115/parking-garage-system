#include "Money.h"

#include <limits>
#include <stdexcept>

Money::Money(long long piastres) : piastres(piastres)
{
    if (piastres < 0)
    {
        throw std::invalid_argument("Money cannot be negative.");
    }
}

long long Money::getPiastres() const
{
    return piastres;
}

Money Money::operator+(const Money& other) const
{
    if (piastres > std::numeric_limits<long long>::max() - other.piastres)
    {
        throw std::overflow_error("Money amount is too large.");
    }

    return Money(piastres + other.piastres);
}

Money Money::operator-(const Money& other) const
{
    if (piastres < other.piastres)
    {
        throw std::invalid_argument("Result cannot be negative.");
    }

    return Money(piastres - other.piastres);
}

Money Money::operator*(int hours) const
{
    if (hours < 0)
    {
        throw std::invalid_argument("Hours cannot be negative.");
    }

    if (hours > 0 &&
        piastres > std::numeric_limits<long long>::max() / hours)
    {
        throw std::overflow_error("Parking fee is too large.");
    }

    return Money(piastres * hours);
}

bool Money::operator==(const Money& other) const
{
    return piastres == other.piastres;
}

bool Money::operator<(const Money& other) const
{
    return piastres < other.piastres;
}

std::ostream& operator<<(std::ostream& out, const Money& money)
{
    long long pounds = money.piastres / 100;
    long long remainingPiastres = money.piastres % 100;

    out << pounds << '.';

    if (remainingPiastres < 10)
    {
        out << '0';
    }

    out << remainingPiastres << " EGP";

    return out;
}