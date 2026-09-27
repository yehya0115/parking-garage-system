#pragma once

#include <ostream>

class Money
{
private:
    long long piastres;

public:
    explicit Money(long long piastres = 0);

    long long getPiastres() const;

    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(int hours) const;

    bool operator==(const Money& other) const;
    bool operator<(const Money& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Money& money);
};