cmake_minimum_required(VERSION 3.27)
project(tank_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW 1
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)
endif()

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v5.5.1
    GIT_SHALLOW 1
)
set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib_cpp)

add_executable(tankBattle
        tankBattle.cpp

        constants/constants.h

        data/point.h
        data/point.cpp
        data/board.h
        data/board.cpp
        data/tank.h
        data/tank.cpp
        data/shell.h
        data/shell.cpp
        data/wall.h
        data/wall.cpp
        data/mine.h
        data/mine.cpp

        logic/char_utils.h
        logic/tank_spawn.h
        logic/tank_spawn.cpp
        logic/tank_movement.h
        logic/tank_movement.cpp
        logic/shell_movement.h
        logic/shell_movement.cpp
        logic/wall_logic.h
        logic/wall_logic.cpp
        logic/collision_rules.h
        logic/collision_rules.cpp
        logic/input_provider.h
        logic/player.h
        logic/player.cpp
        logic/human_player.h
        logic/human_player.cpp
        logic/computer_player.h
        logic/computer_player.cpp

        handlers/raylib_context.h
        handlers/raylib_context.cpp
        handlers/raylib_input_provider.h
        handlers/raylib_input_provider.cpp
        handlers/scene_renderer.h
        handlers/scene_renderer.cpp
        handlers/hud_renderer.h
        handlers/hud_renderer.cpp
        handlers/menu_screens.h
        handlers/menu_screens.cpp
        handlers/game_controller.h
        handlers/game_controller.cpp
)

target_include_directories(tankBattle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tankBattle PRIVATE raylib_cpp raylib)
