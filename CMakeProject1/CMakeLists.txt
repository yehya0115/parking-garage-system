add_executable(CMakeProject1
    "CMakeProject1.cpp"
    "CMakeProject1.h"
    "Money.cpp"
    "Money.h"
    "Time.cpp"
    "Time.h"
    "SlotSize.h"
    "Vehicle.cpp"
    "Vehicle.h"
    "Vehicles.cpp"
    "Vehicles.h"
    "ParkingSlot.cpp"
    "ParkingSlot.h"
    "Floor.cpp"
    "Floor.h"
    "Ticket.cpp"
    "Ticket.h"
    "GarageExceptions.h"
    "Garage.h"
    "Garage.cpp"
    "ConsoleMenu.h"
    "ConsoleMenu.cpp"
)

set_target_properties(CMakeProject1 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    string(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    target_compile_options(CMakeProject1 PRIVATE /W4)
else()
    target_compile_options(CMakeProject1 PRIVATE -Wall -Wextra)
endif()