cmake_minimum_required(VERSION 3.31)
project(tank_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(tank_game tankBattle.cpp
        point.cpp
        point.h
        game.cpp
        game.h
        tank.cpp
        tank.h
        board.cpp
        board.h
        general.h
)
