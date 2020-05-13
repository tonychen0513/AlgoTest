cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file( GLOB SOURCE_FILE "*.cpp")
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests ${SOURCE_FILE})
target_link_libraries(runTests ${GTEST_LIBRARIES} gmock_main pthread)