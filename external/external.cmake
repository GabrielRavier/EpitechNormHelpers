# cppast
message(STATUS "Getting cppast via submodule")
execute_process(
    COMMAND
        git submodule update --init -- external/cppast
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/cppast EXCLUDE_FROM_ALL)

# cppitertools
message(STATUS "Getting cppitertools via submodule")
execute_process(
    COMMAND
        git submodule update --init -- external/cppitertools
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
)
set(ENV{cppitertools_INSTALL_CMAKE_DIR} "share") # This is to avoid a stupid warning
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/cppitertools EXCLUDE_FROM_ALL)
