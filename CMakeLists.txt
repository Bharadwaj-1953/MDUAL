cmake_minimum_required(VERSION 3.10)
project(MDUAL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -fopenmp")

# Add source files
add_executable(mdual
    src/test/testLoad.cpp
    # ... other source files ...
)

# Find OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(mdual PUBLIC OpenMP::OpenMP_CXX)
endif()
