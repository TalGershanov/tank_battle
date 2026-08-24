cmake_minimum_required(VERSION 3.27)
project(tank_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(tankBattle
        tankBattle.cpp
        point.cpp
        point.h
        game.cpp
        game.h
        tank.cpp
        tank.h
        board.cpp
        board.h
        general.h
        general.cpp
        shell.cpp
        shell.h
        wall.cpp
        wall.h
        mine.h
        mine.cpp
        player.h
        player.cpp
)
