#include "handlers/raylib_input_provider.h"
#include "raylib-cpp.hpp"

void RaylibInputProvider::pollFrame() {
    // All gameplay key bindings are single uppercase letters, so polling
    // A-Z each frame covers every key HumanPlayer ever asks about.
    // IsKeyPressed (not IsKeyDown) is latched here: it fires on exactly one
    // frame per physical press, so a single press can only ever land in one
    // tick's window, even if its release spills slightly past a tick edge.
    for (int key = 'A'; key <= 'Z'; ++key) {
        if (IsKeyPressed(key)) {
            latched[key] = true;
        }
    }
}

void RaylibInputProvider::clearLatch() {
    for (bool& b : latched) b = false;
}

bool RaylibInputProvider::isKeyDown(int keyCode) const {
    if (keyCode >= 0 && keyCode < kLatchSize && latched[keyCode]) {
        return true;
    }
    return IsKeyDown(keyCode);
}
