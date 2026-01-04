cmake_minimum_required(VERSION 3.10)
project(BalagurFateEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp") 

add_library(balagur_core STATIC ${CORE_SOURCES})

target_include_directories(balagur_core PUBLIC include)

set(EDITOR_MAIN_SOURCE "${PROJECT_SOURCE_DIR}/src/main.cpp")
add_executable(balagur_editor ${EDITOR_MAIN_SOURCE})

target_link_libraries(balagur_editor PRIVATE balagur_core)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests tests/tests.cpp)

target_include_directories(unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest_main
    balagur_core
)

include(GoogleTest)
gtest_discover_tests(unit_tests)