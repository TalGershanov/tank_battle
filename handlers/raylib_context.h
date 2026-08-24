#pragma once
#include "raylib-cpp.hpp"

// RAII wrapper around the raylib window (InitWindow/CloseWindow via raylib-cpp).
class RaylibContext {
public:
    RaylibContext();
    bool shouldQuit() const;

private:
    raylib::Window window;
};
