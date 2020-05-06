cmake_minimum_required(VERSION 2.6)

option(LTO "Enable link-time optimisation" OFF)

project(epitech-norm-helper CXX)

add_executable(epitech-norm-helper
    "src/main.cpp"
    "src/diagnostic.cpp"
    "src/diagnostic.hpp"
    "src/options.cpp"
    "src/options.hpp"
    "src/program.cpp"
    "src/program.hpp"
    "src/checks/o1.cpp"
    "src/checks/o1.hpp")

# LTO support
if(LTO)
    include(CheckIPOSupported)

    check_ipo_supported(RESULT result)

    if(result)
        set_target_properties(epitech-norm-helper INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# Make C++20 mandatory
set_target_properties(epitech-norm-helper PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

# fmt library
find_package(fmt)
target_link_libraries(epitech-norm-helper fmt::fmt)

# Add 'external' directory as include directory
target_include_directories(epitech-norm-helper PRIVATE external)
