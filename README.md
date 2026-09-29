# Parking Garage System

A C++17 console application for managing a multi-floor parking garage.

## Author

Yehya Ahmed Mohamed Hassan — Individual project.

## Implemented Features

- Park motorcycles, cars, and trucks.
- Select the smallest available suitable slot across all floors.
- Issue numbered tickets containing the license plate, entry time, and slot coordinates.
- Retrieve vehicles using active ticket numbers.
- Calculate parking fees by vehicle type and duration.
- Display an occupancy map for each floor.
- View active ticket details.
- Reject duplicate plates, invalid tickets, invalid times, and null vehicles.
- Validate numeric console input.
- Run an automatic demonstration without user input using `--demo`.

## Garage Rules

The interactive application creates two floors.
Each floor contains two rows and three columns, giving 12 slots in total.

Floor, row, and column numbering starts at 0.

Each row repeats the slot sizes Small, Medium, and Large.

| Vehicle | Required size | Hourly rate |
| --- | --- | --- |
| Motorcycle | Small | 10 EGP |
| Car | Medium | 20 EGP |
| Truck | Large | 40 EGP |

- A larger slot can accept a smaller vehicle.
- Allocation searches all floors for the smallest suitable free slot.
- Equal-sized candidates are selected in floor, row, then column order.
- Each started hour is charged as a full hour.
- Zero minutes costs zero.
- Exit time cannot be earlier than entry time.
- Duplicate license plates inside the garage are rejected.
- License plate comparisons are case-sensitive.
- The console removes leading and trailing whitespace from license plates.
- Retrieving a vehicle removes its active ticket.
- Data is held in memory and is not saved between program runs.
- The application calculates fees; it does not process payments.

## Requirements

- A C++17 compiler.
- CMake 3.15 or later.
- A build tool compatible with the selected CMake generator.

Development and execution have been verified using Visual Studio 2022
with the x64 Debug configuration on Windows.

## Build and Run on Windows

1. Open the repository root folder in Visual Studio.
2. Enable CMake support if prompted.
3. Select the x64 Debug configuration.
4. Wait for CMake generation to finish.
5. Use Build > Rebuild All.
6. Run the application using Ctrl+F5.

The repository root is the folder containing the top-level
`CMakeLists.txt` and `CMakePresets.json`.

After building, run the interactive application from PowerShell
in the repository root:

```powershell
.\out\build\x64-debug\CMakeProject1\CMakeProject1.exe
```

Run the automatic demonstration:

```powershell
.\out\build\x64-debug\CMakeProject1\CMakeProject1.exe --demo
```

Check the exit code immediately afterward:

```powershell
$LASTEXITCODE
```

A successful demo prints:

```text
All demo checks passed.
```

and exits with code `0`.

Unsupported command-line arguments print usage information and
exit with code `1`.

If building from a terminal, use an x64 developer environment.
An x86 library environment must not be mixed with the x64 build.

## Interactive Menu

| Option | Action |
| --- | --- |
| 1 | Park a vehicle |
| 2 | Retrieve a vehicle |
| 3 | Calculate a parking fee without retrieving |
| 4 | Show occupancy maps |
| 5 | Show active ticket details |
| 0 | Exit |

Time is entered as a day number, hour, and minute.

For example, entry at Day 0 23:30 and exit at Day 1 01:00
produce a duration of 90 minutes and two charged hours.

Retrieval displays the fee and asks for confirmation.
Cancelling leaves the vehicle parked and its ticket active.

Map symbols:

- `S`: Small slot.
- `M`: Medium slot.
- `L`: Large slot.
- `X`: Occupied.
- `.`: Empty.

## Project Structure

Source files are stored in `CMakeProject1/`.

| Files | Responsibility |
| --- | --- |
| `Money.h`, `Money.cpp` | Money values and arithmetic |
| `Time.h`, `Time.cpp` | Time validation, comparison, and duration |
| `SlotSize.h` | Slot-size enumeration |
| `Vehicle.h`, `Vehicle.cpp` | Abstract vehicle interface and plate validation |
| `Vehicles.h`, `Vehicles.cpp` | Motorcycle, Car, and Truck implementations |
| `ParkingSlot.h`, `ParkingSlot.cpp` | Slot validation and vehicle ownership |
| `Floor.h`, `Floor.cpp` | Two-dimensional slot grid |
| `Ticket.h`, `Ticket.cpp` | Ticket details and static numbering |
| `GarageExceptions.h` | Custom exception types |
| `Garage.h`, `Garage.cpp` | Allocation, active tickets, fees, and retrieval |
| `ConsoleMenu.h`, `ConsoleMenu.cpp` | Interactive input and output |
| `Demo.h`, `Demo.cpp` | Noninteractive demonstration and checks |
| `CMakeProject1.cpp` | Program entry point and command-line selection |

## Class Design and Public Interfaces

### Money

Stores a nonnegative integer number of piastres, avoiding
floating-point rounding in monetary values.

Main public operations:

```cpp
explicit Money(long long piastres = 0);
long long getPiastres() const;

Money operator+(const Money& other) const;
Money operator-(const Money& other) const;
Money operator*(int hours) const;

bool operator==(const Money& other) const;
bool operator<(const Money& other) const;

friend std::ostream& operator<<(std::ostream& out, const Money& money);
```

Invalid negative amounts and negative results are rejected.
Addition and multiplication check for overflow.

### Time

Stores a nonnegative total number of minutes within the range of `int`.

```cpp
Time(int day = 0, int hour = 0, int minute = 0);

int operator-(const Time& other) const;
bool operator==(const Time& other) const;
bool operator<(const Time& other) const;

friend std::ostream& operator<<(std::ostream& out, const Time& time);
```

The constructor validates the day, hour, minute, and total range.

Subtracting two times returns a signed difference in minutes.
Garage operations separately reject an exit before entry.

### Vehicle and Derived Classes

`Vehicle` is an abstract base class.

```cpp
explicit Vehicle(const std::string& plate);
virtual ~Vehicle() = default;

const std::string& getPlate() const;

virtual std::string getType() const = 0;
virtual Money getHourlyRate() const = 0;
virtual SlotSize getRequiredSize() const = 0;
```

`Motorcycle`, `Car`, and `Truck` implement the virtual operations.
Garage code works through the base-class interface.

### ParkingSlot

A slot exclusively owns its parked vehicle.

```cpp
explicit ParkingSlot(SlotSize size);

SlotSize getSize() const;
bool isOccupied() const;
bool canFit(const Vehicle& vehicle) const;
const Vehicle* getVehicle() const;

void park(std::unique_ptr<Vehicle>& vehicle);
std::unique_ptr<Vehicle> retrieve();
```

`canFit()` checks size compatibility.
Availability is checked separately with `isOccupied()`.

`getVehicle()` returns a non-owning pointer for inspection.

Parking validates the input before moving ownership.
Retrieval moves ownership back to the caller.

### Floor

Contains a fixed-size grid:

```cpp
std::vector<std::vector<ParkingSlot>>
```

Main public operations:

```cpp
Floor(int number, int rows, int columns);

int getNumber() const;
int getRows() const;
int getColumns() const;
int getOccupiedCount() const;

ParkingSlot& getSlot(int row, int column);
const ParkingSlot& getSlot(int row, int column) const;
```

Invalid dimensions and out-of-range coordinates are rejected.

### Ticket

Stores a number, license plate, entry time, and slot coordinates.

```cpp
Ticket(int floor, int row, int column,
       const std::string& plate, const Time& entryTime);

int getNumber() const;
int getFloor() const;
int getRow() const;
int getColumn() const;

const std::string& getPlate() const;
const Time& getEntryTime() const;
```

A private static counter assigns numbers to newly constructed tickets.

Copying a ticket preserves its number and represents the same ticket;
it does not issue a new ticket.

Ticket numbering is shared within the running process and resets
when the application restarts.

### Garage

Owns floors and stores active tickets in `std::map<int, Ticket>`.

```cpp
Garage(int floorCount, int rows, int columns);

Ticket parkVehicle(std::unique_ptr<Vehicle>& vehicle,
                   const Time& entryTime);

Money calculateFee(int ticketNumber, const Time& exitTime) const;

std::unique_ptr<Vehicle> retrieveVehicle(
    int ticketNumber, const Time& exitTime);

const Ticket& getTicket(int ticketNumber) const;
const Floor& getFloor(int index) const;

int getFloorCount() const;
int getOccupiedCount() const;
```

Parking rejects duplicate plates and finds the smallest suitable slot.

The ticket is inserted before vehicle ownership changes.
If slot parking throws, the inserted ticket is removed.

Retrieval validates the ticket, exit time, and fee before
moving the vehicle and removing the ticket.

A reference returned by `getTicket()` must not be used after
that ticket is removed.

## Ownership and Copy/Move Behaviour

- Vehicles are created with `std::make_unique`.
- Before parking, the caller owns the vehicle.
- Successful parking moves ownership into a slot.
- The caller's pointer becomes null after that move.
- Failed parking preserves caller ownership.
- Retrieval moves ownership back to the caller.
- Tickets store coordinates, not owning pointers.
- Temporary raw pointers used for inspection or slot selection do not own objects.
- No manual `new` or `delete` is used.
- Containers and smart pointers release their resources automatically.

`Money`, `Time`, and `Ticket` use compiler-generated value operations.

`ParkingSlot` cannot be copied because it contains a `unique_ptr`,
but it can be moved.

`Floor` and `Garage` explicitly delete copying and default their
move operations.

The demo demonstrates ticket copying and vehicle ownership transfer.

## Exceptions

Custom exceptions derive from `std::runtime_error`:

- `GarageFullException`: no free suitable slot exists.
- `InvalidTicketException`: the ticket is missing or already used.

Standard exceptions report invalid inputs, coordinate errors,
arithmetic overflow, and inconsistent internal state.

The menu catches operation errors, displays their messages,
and allows the user to continue.

## SOLID Examples

### Single Responsibility Principle

Responsibilities are separated:

- Money handles monetary values.
- Time handles time values.
- ParkingSlot handles ownership of one parked vehicle.
- Garage coordinates allocation, tickets, and retrieval.
- ConsoleMenu handles user interaction.

### Open/Closed Principle

Garage obtains rates and required sizes through virtual
Vehicle functions.

A new vehicle type using the existing slot sizes can be added
without changing Garage's fee calculation or allocation algorithm.

The console vehicle-selection menu would still need an option
for creating that new type.

## Automatic Demo

Run with `--demo`.

The demo uses a separate garage with two floors, one row per floor,
and three columns per row.

It checks:

- Empty initial state.
- Ownership transfer during parking and retrieval.
- Smallest suitable slot selection across floors.
- Distinct newly issued ticket numbers.
- Preservation of ticket number when copied.
- Vehicle-specific slot sizes and fees.
- Overnight duration calculation.
- Zero-duration and partial-hour charges.
- Duplicate plate rejection.
- Earlier-exit rejection.
- Removal of used tickets.
- Unavailable suitable slots.
- Null vehicle rejection.

It prints occupancy maps before and after the demonstrated operations.

Four vehicles intentionally remain parked at the end.
They are destroyed automatically when the demo garage is destroyed.

The Windows demo has completed successfully with exit code `0`.

## Manual Test Cases

Unless stated otherwise, start each case with a fresh interactive run.
Use the actual ticket number printed by the application.

These are reproducible test cases with expected results.
The table is not a claim that every case has been manually verified.

| No. | Action | Expected result |
| --- | --- | --- |
| 1 | Display the map immediately after startup | All 12 slots are empty; total occupied is 0 |
| 2 | Park a car with plate CAR-100 | A Medium slot is selected and a ticket is printed |
| 3 | Park a motorcycle in a fresh garage | A Small slot is selected |
| 4 | Park a truck in a fresh garage | A Large slot is selected |
| 5 | Park another vehicle with an already parked plate | Duplicate plate error; occupancy unchanged |
| 6 | Enter a blank or whitespace-only plate | Input is rejected and the plate is requested again |
| 7 | Enter abc or 2abc at the main menu | Input is rejected; the menu remains usable |
| 8 | Enter hour 24 or minute 60 | Input is rejected and a valid number is requested |
| 9 | Park a car at Day 0 10:00; calculate fee at 10:00 | Fee is 0.00 EGP |
| 10 | Park a car at Day 0 10:00; calculate fee at 10:01 | Fee is 20.00 EGP |
| 11 | Park a car at Day 0 10:00; calculate fee at 11:00 | Fee is 20.00 EGP |
| 12 | Park a car at Day 0 10:00; calculate fee at 11:01 | Fee is 40.00 EGP |
| 13 | Park a car at Day 0 23:30; calculate fee at Day 1 01:00 | Fee is 40.00 EGP |
| 14 | Attempt retrieval with exit before entry | Error; vehicle remains parked and ticket remains active |
| 15 | Look up a ticket number that was never issued | Invalid ticket error |
| 16 | Retrieve a vehicle, then reuse its ticket | First retrieval succeeds; reuse is rejected |
| 17 | Cancel retrieval at the confirmation prompt | Vehicle remains parked and ticket remains active |
| 18 | Park four trucks with different plates, then a fifth | Fifth truck is rejected because all Large slots are occupied |
| 19 | Retrieve a car, then park another car | Freed Medium slot can be reused |
| 20 | Calculate a fee without choosing retrieval | Vehicle and ticket remain active |
| 21 | Enter option 0 | Application exits normally |

## Build Warnings and Remaining Validation

CMake enables:

- `/W4` for MSVC.
- `-Wall -Wextra` for other supported compilers.

Windows rebuilding and automatic demo execution have been verified
during development.

Remaining validation before final submission:

- Final clean rebuild of the completed project.
- Complete and record the manual test cases.
- Build and run on Linux.
- Run Valgrind and record its actual output.

No claim of a successful Valgrind run is made yet.

## Git Workflow

Development uses feature branches and GitHub pull requests.

Implemented stages include:

- Initial design.
- Money and Time.
- Vehicles.
- Parking slots.
- Floors.
- Tickets.
- Garage management.
- Console menu.
- Automatic demo.

Repository:

https://github.com/yehya0115/parking-garage-system