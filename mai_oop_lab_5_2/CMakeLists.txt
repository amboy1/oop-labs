cmake_minimum_required(VERSION 3.15)
project(MyStackProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование
include(CTest)

# Создаем исполняемый файл
add_executable(my_stack_main src/main.cpp)

# Устанавливаем пути для заголовочных файлов
target_include_directories(my_stack_main PRIVATE include)

# Создаем тестовый исполняемый файл
add_executable(my_stack_test src/test.cpp)
target_include_directories(my_stack_test PRIVATE include)

# Добавляем тесты в CTest
add_test(NAME MyStackTest COMMAND my_stack_test)

# Опционально: включаем санитайзеры для отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(my_stack_main PRIVATE -fsanitize=address,undefined)
    target_compile_options(my_stack_test PRIVATE -fsanitize=address,undefined)
    target_link_options(my_stack_main PRIVATE -fsanitize=address,undefined)
    target_link_options(my_stack_test PRIVATE -fsanitize=address,undefined)
endif()