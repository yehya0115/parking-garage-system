add_executable(CMakeProject1
    "CMakeProject1.cpp"
    "CMakeProject1.h"
    "Money.cpp"
    "Money.h"
)

set_target_properties(CMakeProject1 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(CMakeProject1 PRIVATE /W4)
else()
    target_compile_options(CMakeProject1 PRIVATE -Wall -Wextra)
endif()