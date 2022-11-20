cmake_minimum_required(VERSION 3.0.0)
project(mdmE VERSION 0.1.0)

include_directories(
    "include/"
    "./src/**/*.h"
)

link_directories(
    "lib/"
)

add_subdirectory("src")

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/src)
file(GLOB DLLS "lib/*.dll")
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/src)
