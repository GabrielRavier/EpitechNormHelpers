# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.12)

option(LTO "Enable link-time optimisation" OFF)

project(epitech-norm-helper CXX)

add_executable(epitech-norm-helper
    "src/main.cpp"
    "src/diagnostic.cpp"
    "include/diagnostic.hpp"
    "src/options.cpp"
    "include/options.hpp"
    "src/program.cpp"
    "include/program.hpp"
    "src/basename.cpp"
    "include/basename.hpp"
    "src/executable-type.cpp"
    "include/executable-type.hpp"
    "src/regex-utils.cpp"
    "include/regex-utils.hpp"
    "src/checks/o1.cpp"
    "include/checks/o1.hpp"
    "src/checks/o2.cpp"
    "include/checks/o2.hpp"
    "src/checks/o3.cpp"
    "include/checks/o3.hpp"
    "include/libgit2wrapper/errors.hpp"
    "include/libgit2wrapper/global.hpp"
    "include/libgit2wrapper/index.hpp"
    "include/libgit2wrapper/repository.hpp")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# LTO support
if(LTO)
    include(CheckIPOSupported)

    check_ipo_supported(RESULT result)

    if(result)
        set_target_properties(epitech-norm-helper INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# Make C++17 mandatory
set_target_properties(epitech-norm-helper
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# fmt library
find_package(fmt REQUIRED)
target_link_libraries(epitech-norm-helper fmt::fmt)

# libgit2 library
find_package(libgit2 REQUIRED)
target_link_libraries(epitech-norm-helper ${LIBGIT2_LIBRARIES})

# Boost.Regex library
find_package(Boost
    COMPONENTS
        regex REQUIRED)

target_link_libraries(epitech-norm-helper ${Boost_LIBRARIES})

# external stuff
include(external/external.cmake)

# cppitertools library
target_link_libraries(epitech-norm-helper cppitertools)

# Add our 'include' directory as an include directory
target_include_directories(epitech-norm-helper PRIVATE include)
