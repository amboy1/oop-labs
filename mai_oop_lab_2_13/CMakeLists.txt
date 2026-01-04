cmake_minimum_required(VERSION 3.10)
project(BinaryProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение Google Test
find_package(GTest REQUIRED)
if (GTest_FOUND)
    message(STATUS "Google Test найден: ${GTEST_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Google Test не найден. Убедитесь, что он установлен.")
endif()

# Тесты
add_executable(binary_test tests/tests.cpp src/binary.cpp)
target_link_libraries(binary_test PRIVATE GTest::GTest GTest::Main pthread)

# Включение тестирования
enable_testing()
add_test(NAME BinaryTests COMMAND binary_test)