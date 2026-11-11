cmake_minimum_required(VERSION 3.16)

project(ALSE_InventarioMotos VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar Qt 6 (Widgets, SQL, Charts)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

# Archivos fuente
set(PROJECT_SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)
