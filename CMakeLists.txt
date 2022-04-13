cmake_minimum_required(VERSION 3.0.0)
project(Orenma4 VERSION 0.1.0)

include_directories(
    "include/"
    "C:/dev/vcpkg/installed/x64-windows/include/"
)

link_directories(
    "lib/"
    "C:/dev/vcpkg/installed/x64-windows/lib/"
)

add_subdirectory("src")
#add_subdirectory("include/soloud")

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/src)
file(GLOB DLLS "lib/*.dll")
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR}/src)
