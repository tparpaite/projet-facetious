cmake_minimum_required(VERSION 2.8)
project(FACETIOUS_PELICAN)
add_definitions(-std=c99 -g -Wall)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_LIBRARY_PATH ${PROJECT_BINARY_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/ADT ${PROJECT_SOURCE_DIR}/include/facetious_pelican)

configure_file(${PROJECT_SOURCE_DIR}/Doxyfile ${PROJECT_BINARY_DIR}/Doxyfile.cmake @ONLY)
add_custom_target(doc COMMAND doxygen ${PROJECT_BINARY_DIR}/Doxyfile.cmake)

add_subdirectory(src)