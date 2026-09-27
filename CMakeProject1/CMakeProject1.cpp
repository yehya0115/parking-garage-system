#include <iostream>
#include <stdexcept>

#include "Money.h"
#include "Time.h"

using namespace std;

int main()
{
    try
    {
        Time entry(0, 23, 30);
        Time exit(1, 1, 0);

        int duration = exit - entry;

        int chargedHours = duration / 60;

        if (duration % 60 != 0)
        {
            ++chargedHours;
        }

        Money hourlyRate(2000);
        Money fee = hourlyRate * chargedHours;

        cout << "Entry: " << entry << endl;
        cout << "Exit: " << exit << endl;
        cout << "Duration: " << duration << " minutes" << endl;
        cout << "Charged hours: " << chargedHours << endl;
        cout << "Fee: " << fee << endl;

        cout << boolalpha;
        cout << "Entry before exit: " << (entry < exit) << endl;
        cout << "Equal entry times: "
            << (entry == Time(0, 23, 30)) << endl;
        cout << "Same-time duration: " << (entry - entry) << endl;
        cout << "Reverse difference: " << (entry - exit) << endl;

        cout << "Testing an invalid hour..." << endl;
        Time invalidTime(0, 24, 0);
        cout << invalidTime << endl;
    }
    catch (const exception& error)
    {
        cout << "Error: " << error.what() << endl;
    }

    return 0;
}