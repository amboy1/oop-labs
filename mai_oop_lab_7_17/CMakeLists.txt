cmake_minimum_required(VERSION 3.15)  # Повышаем версию для лучшей поддержки C++17
project(BalagurFate3 VERSION 1.0)    # Меняем имя проекта

# Настройки компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройки вывода
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Флаги компиляции для многопоточности и отладки
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -pthread)
    add_link_options(-pthread)
endif()

# Включаем поддержку многопоточности
find_package(Threads REQUIRED)

# Собираем список исходных файлов библиотеки
file(GLOB_RECURSE CORE_SOURCES 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*/*/*.cpp"
)

# Убираем main.cpp из библиотеки
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Основная библиотека
add_library(balagur_core STATIC ${CORE_SOURCES})

# Директории включаемых файлов
target_include_directories(balagur_core 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src  # Для доступа к файлам внутри src
)

# Связываем библиотеку с потоками
target_link_libraries(balagur_core PRIVATE Threads::Threads)

# Основное приложение
add_executable(balagur_editor src/main.cpp)
target_link_libraries(balagur_editor PRIVATE balagur_core)

# Если нужно, добавляем приложение для игры (новый main из примера)
# Для этого создайте новый файл src/game_main.cpp с кодом из примера выше
if(EXISTS "${PROJECT_SOURCE_DIR}/src/game_main.cpp")
    add_executable(balagur_game src/game_main.cpp)
    target_link_libraries(balagur_game PRIVATE balagur_core)
    set_target_properties(balagur_game PROPERTIES
        OUTPUT_NAME "balagur_fate3_game"
    )
endif()

# Настройки для Google Test (если нужны тесты)
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    
    # Для Windows
    if(WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    
    FetchContent_MakeAvailable(googletest)
    
    enable_testing()
    
    # Тесты
    if(EXISTS "${PROJECT_SOURCE_DIR}/tests/tests.cpp")
        add_executable(unit_tests tests/tests.cpp)
        
        target_include_directories(unit_tests PRIVATE
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src
        )
        
        target_link_libraries(unit_tests PRIVATE
            GTest::gtest_main
            balagur_core
        )
        
        include(GoogleTest)
        gtest_discover_tests(unit_tests)
    else()
        message(WARNING "Test file tests/tests.cpp not found. Tests will not be built.")
    endif()
endif()

# Установка (опционально)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "Installation directory" FORCE)
endif()

install(TARGETS balagur_editor
    RUNTIME DESTINATION bin
)

if(TARGET balagur_game)
    install(TARGETS balagur_game
        RUNTIME DESTINATION bin
    )
endif()

install(TARGETS balagur_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Сообщение о конфигурации
message(STATUS "Project configured successfully")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Tests enabled: ${BUILD_TESTS}")