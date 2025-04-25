cmake_minimum_required(VERSION 3.18)

project(NetWorkSamples)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(chap-1)
add_subdirectory(chap-2)
add_subdirectory(chap-3)
add_subdirectory(chap-4)
add_subdirectory(chap-5)
add_subdirectory(chap-6)
add_subdirectory(chap-7)
add_subdirectory(chap-8)