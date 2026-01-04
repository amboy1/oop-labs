cmake_minimum_required(VERSION 3.10)
project(lab1_7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(breakChocolate src/breakChocolate.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE breakChocolate)

enable_testing()

add_executable(breakChocolateTests tests/breakChocolateTests.cpp)
target_link_libraries(breakChocolateTests PRIVATE breakChocolate GTest::GTest GTest::Main)

add_test(NAME breakChocolateTests COMMAND breakChocolateTests)