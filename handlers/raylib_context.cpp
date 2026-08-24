#include "handlers/raylib_context.h"
#include "constants/constants.h"

RaylibContext::RaylibContext() : window(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX, WINDOW_TITLE) {
    // raylib closes the window on ESC by default; ESC is used for the pause
    // menu here, so window-close must only come from the OS close button.
    raylib::Window::SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);
}

bool RaylibContext::shouldQuit() const {
    return window.ShouldClose();
}
