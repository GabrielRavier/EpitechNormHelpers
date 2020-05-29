# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.12)

option(LTO "Enable link-time optimisation" OFF)

project(epitech-norm-tool CXX)

add_executable(epitech-norm-tool
    "src/main.cpp"
    "src/basename.cpp"
    "include/basename.hpp"
    "src/diagnostic.cpp"
    "include/diagnostic.hpp"
    "src/executable-type.cpp"
    "include/executable-type.hpp"
    "src/file-utils.cpp"
    "include/file-utils.hpp"
    "src/managers.cpp"
    "include/managers.hpp"
    "include/managers-fwd.hpp"
    "src/options.cpp"
    "include/options.hpp"
    "include/options-fwd.hpp"
    "src/program.cpp"
    "include/program.hpp"
    "src/regex-utils.cpp"
    "include/regex-utils.hpp"
    "src/checks/o1.cpp"
    "include/checks/o1.hpp"
    "src/checks/o2.cpp"
    "include/checks/o2.hpp"
    "src/checks/o3.cpp"
    "include/checks/o3.hpp"
    "src/checks/o4.cpp"
    "include/checks/o4.hpp"
    "src/checks/g1.cpp"
    "include/checks/g1.hpp"
    "src/checks/g2.cpp"
    "include/checks/g2.hpp"
    "src/checks/g3.cpp"
    "include/checks/g3.hpp"
    "src/checks/g4.cpp"
    "include/checks/g4.hpp"
    "src/checks/g5.cpp"
    "include/checks/g5.hpp"
    "src/checks/f1.cpp"
    "include/checks/f1.hpp"
    "src/checks/f2.cpp"
    "include/checks/f2.hpp"
    "src/checks/f3.cpp"
    "include/checks/f3.hpp"
    "src/checks/f4.cpp"
    "include/checks/f4.hpp"
    "include/checks/stubs.hpp"
    "include/checks/checks.hpp"
    "src/checks/checks.cpp"
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
        set_target_properties(epitech-norm-tool INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# Make C++17 mandatory
set_target_properties(epitech-norm-tool
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# fmt library
find_package(fmt REQUIRED)
target_link_libraries(epitech-norm-tool fmt::fmt)

# libgit2 library
find_package(libgit2 REQUIRED)
target_link_libraries(epitech-norm-tool ${LIBGIT2_LIBRARIES})

# Boost.Regex library
find_package(Boost
    COMPONENTS
        regex REQUIRED)

target_link_libraries(epitech-norm-tool ${Boost_LIBRARIES})

# external stuff
include(external/external.cmake)

# cppitertools library
target_link_libraries(epitech-norm-tool cppitertools)

# cppast library
target_link_libraries(epitech-norm-tool cppast)

# Add our 'include' directory as an include directory
target_include_directories(epitech-norm-tool PRIVATE include)
