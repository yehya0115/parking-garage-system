#include <iostream>
#include <stdexcept>

#include "Money.h"

using namespace std;

int main()
{
    try
    {
        Money hourlyRate(2000);
        Money extra(505);
        Money fee = hourlyRate * 3;

        cout << "Hourly rate: " << hourlyRate << endl;
        cout << "Fee for 3 hours: " << fee << endl;
        cout << "Addition: " << hourlyRate + extra << endl;
        cout << "Subtraction: " << hourlyRate - extra << endl;

        cout << boolalpha;
        cout << "Equal to 60 EGP: " << (fee == Money(6000)) << endl;
        cout << "Extra is smaller: " << (extra < hourlyRate) << endl;
        cout << "Zero hours: " << hourlyRate * 0 << endl;

        cout << "Testing a negative amount..." << endl;
        Money invalidAmount(-100);
        cout << invalidAmount << endl;
    }
    catch (const exception& error)
    {
        cout << "Error: " << error.what() << endl;
    }

    return 0;
}