# Parking Garage System



A C++17 console project for managing a multi-floor parking garage.



## Author



Yehya Ahmed Mohamed Hassan - Individual project.



## Current Progress



- CMake project configured for C++17.

- Basic console program runs successfully.

- Git repository and .gitignore created.

- Garage features are not implemented yet.



## Planned Features



- Park cars, motorcycles, and trucks.

- Choose a free slot with a suitable size.

- Issue a ticket with a unique number and entry time.

- Retrieve a vehicle using its ticket number.

- Calculate parking fees by vehicle type and parking duration.

- Display an occupancy map for each floor.

- Handle invalid input, invalid tickets, and unavailable slots.

- Provide a demo mode without menu input.



## Initial Rules



- Slot sizes: Small, Medium, Large.

- Motorcycle requires Small, Car requires Medium, Truck requires Large.

- A larger slot can accept a smaller vehicle.

- Use the smallest available suitable slot.

- Planned hourly rates: Motorcycle 10 EGP, Car 20 EGP, Truck 40 EGP.

- Each started hour is charged as a full hour.

- Zero minutes costs zero.

- Time includes a day number, hour, and minute.

- Exit time cannot be earlier than entry time.

- Duplicate license plates inside the garage are rejected.

- Data is kept in memory during the program session.



## Class Design and Planned Public Interfaces



The signatures below describe the intended interface.

They are a design plan, not complete class definitions.



### Money



Stores money as an integer number of piastres.



- explicit Money(long long piastres = 0)

- long long getPiastres() const

- Money operator+(const Money& other) const

- Money operator-(const Money& other) const

- Money operator*(int hours) const

- bool operator==(const Money& other) const

- bool operator&lt;(const Money& other) const

- friend std::ostream& operator&lt;&lt;(std::ostream& out, const Money& money)



### Time



Stores a time using day number, hour, and minute.

Day numbering starts at 0.



- Time(int day = 0, int hour = 0, int minute = 0)

- int operator-(const Time& other) const

- bool operator==(const Time& other) const

- bool operator&lt;(const Time& other) const

- friend std::ostream& operator&lt;&lt;(std::ostream& out, const Time& time)



Subtracting two Time values returns the difference in minutes.



### Vehicle - Abstract Base Class



Stores the license plate and defines vehicle-specific behaviour.



- explicit Vehicle(const std::string& plate)

- virtual ~Vehicle() = default

- const std::string& getPlate() const

- virtual std::string getType() const = 0

- virtual Money getHourlyRate() const = 0

- virtual SlotSize getRequiredSize() const = 0



### Car, Motorcycle, Truck



Each class inherits from Vehicle.



- Constructor accepting const std::string& plate

- std::string getType() const override

- Money getHourlyRate() const override

- SlotSize getRequiredSize() const override



### ParkingSlot



Owns a parked vehicle through std::unique_ptr&lt;Vehicle&gt;.



- explicit ParkingSlot(SlotSize size)

- SlotSize getSize() const

- bool isOccupied() const

- bool canFit(const Vehicle& vehicle) const

- const Vehicle* getVehicle() const

- void park(std::unique_ptr&lt;Vehicle&gt;& vehicle)

- std::unique_ptr&lt;Vehicle&gt; retrieve()



getVehicle() returns a non-owning pointer for inspection.

park() moves ownership into the slot after validation.

retrieve() moves ownership back to the caller.



### Floor



Contains a two-dimensional grid of ParkingSlot objects:

std::vector&lt;std::vector&lt;ParkingSlot&gt;&gt;.



- Floor(int number, int rows, int columns)

- int getNumber() const

- int getRows() const

- int getColumns() const

- int getOccupiedCount() const

- ParkingSlot& getSlot(int row, int column)

- const ParkingSlot& getSlot(int row, int column) const



Each row contains Small, Medium, and Large slots in a repeating pattern.

The grid dimensions remain fixed after construction.



### Ticket



Stores the ticket number, entry time, license plate,

and floor/row/column coordinates.



- Ticket(int floor, int row, int column,

&#x20;        const std::string& plate, const Time& entryTime)

- int getNumber() const

- int getFloor() const

- int getRow() const

- int getColumn() const

- const std::string& getPlate() const

- const Time& getEntryTime() const



A private static counter generates ticket numbers.

Coordinates identify a slot without owning it.



### Garage



Manages floors and active tickets.



- Garage(int floorCount, int rows, int columns)

- Ticket parkVehicle(std::unique_ptr&lt;Vehicle&gt;& vehicle,

&#x20;                    const Time& entryTime)

- Money calculateFee(int ticketNumber, const Time& exitTime) const

- std::unique_ptr&lt;Vehicle&gt; retrieveVehicle(int ticketNumber,

&#x20;                                         const Time& exitTime)

- const Ticket& getTicket(int ticketNumber) const

- const Floor& getFloor(int index) const

- int getFloorCount() const

- int getOccupiedCount() const



Retrieving a vehicle validates the ticket and exit time,

moves the vehicle out, and removes the active ticket.

The menu calculates and displays the fee before retrieval.



### Custom Exceptions



GarageFullException and InvalidTicketException derive

from std::runtime_error.



Each has a constructor accepting a descriptive message.

They use the inherited what() function.



## Memory Management Plan



- Create vehicles with std::make_unique.

- Use std::unique_ptr&lt;Vehicle&gt; for exclusive ownership.

- Move ownership into a slot when parking.

- Move ownership back to the caller when retrieving.

- If parking fails, the caller keeps ownership.

- Tickets store slot coordinates, not owning pointers.

- Do not use raw new or delete.

- Use standard containers and Rule of Zero where appropriate.

- Demonstrate ownership changes in demo mode.



## SOLID Plan



- Single Responsibility: separate money, time, vehicles,

&#x20; parking management, and console interaction.

- Open/Closed: Garage uses Vehicle virtual functions for

&#x20; rates and sizes, allowing new vehicle types without

&#x20; changing fee calculation.



## Remaining Work



- Implement the planned classes.

- Add the console menu and demo mode.

- Document verified build and run commands.

- Add at least 10 manual test cases with expected results.

- Check compiler warnings and run Valgrind.

- Record meaningful commits and merge feature branches

&#x20; through GitHub pull requests.


