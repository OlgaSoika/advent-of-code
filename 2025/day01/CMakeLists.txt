cmake_minimum_required(VERSION 3.16)

# Each day folder is a completely independent small project
project(aoc_day LANGUAGES CXX)

# Use modern C++ (adjust if you prefer C++17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically detect all solution-related source files.
# This includes:
#   solution.cpp
#   solution_final.cpp
#   solution_part2.cpp
#   solution_part2_final.cpp
#
# This allows you to simply add a new file, and it will
# automatically build without modifying CMakeLists.
file(GLOB DAY_SOURCES CONFIGURE_DEPENDS "solution*.cpp")

if(NOT DAY_SOURCES)
    message(WARNING "No source files found matching 'solution*.cpp'")
endif()

# For each detected .cpp file, create a separate executable.
# Example:
#   solution.cpp              → aoc_solution
#   solution_part2.cpp        → aoc_solution_part2
#   solution_final.cpp        → aoc_solution_final
#
# The target names are always unique and predictable.
foreach(src ${DAY_SOURCES})
    # Extract filename without extension
    get_filename_component(name_we ${src} NAME_WE)

    # Create a clean and unique target name
    # Format: aoc_<filename>
    # Example: aoc_solution_part2_final
    set(target_name aoc_${name_we})

    # Define the executable target
    add_executable(${target_name} ${src})
endforeach()