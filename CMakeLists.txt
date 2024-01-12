cmake_minimum_required(VERSION 3.27)
project(Cadillacs_and_Dinosaurs_Game)

set(CMAKE_CXX_STANDARD 23)

add_executable(Cadillacs_and_Dinosaurs_Game Game.cpp
        Game.h
        Player.cpp
        Player.h
        Character.cpp
        Character.h
        Enemy.cpp
        Enemy.h
        Sprite.cpp
        Sprite.h
        Graphics.cpp
        Graphics.h
        Utils.cpp
        Utils.h)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR E:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(E:/SFML/include)
# Link msimg32 library and sfml library

target_link_libraries(Cadillacs_and_Dinosaurs_Game PRIVATE msimg32 sfml-system sfml-window sfml-graphics sfml-audio)
# Set compiler options for C++23
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Cadillacs_and_Dinosaurs_Game PRIVATE -std=c++23)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Cadillacs_and_Dinosaurs_Game PRIVATE /std:c++23)
endif()
