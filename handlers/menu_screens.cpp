#include "handlers/menu_screens.h"
#include "raylib-cpp.hpp"
#include "constants/constants.h"
#include <string>
#include <vector>

namespace {

// Draws `lines` every frame and blocks until a key is pressed or the window
// is closed. Returns the raylib key code pressed, or 0 if the window closed.
int waitForKeyPress(RaylibContext& context, const std::vector<std::string>& lines) {
    // Discard any leftover key-press events queued up before this screen
    // started waiting (e.g. from gameplay, which never drains this queue),
    // so only a genuinely fresh press counts.
    while (GetKeyPressed() != 0) {}

    while (!context.shouldQuit()) {
        int key = GetKeyPressed();

        BeginDrawing();
        ClearBackground(BLACK);

        float totalHeight = static_cast<float>(lines.size()) * HUD_LINE_HEIGHT_PX;
        float startY = (WINDOW_HEIGHT_PX - totalHeight) / 2.0f;

        for (size_t i = 0; i < lines.size(); i++) {
            Vector2 lineSize = MeasureTextEx(GetFontDefault(), lines[i].c_str(),
                                              static_cast<float>(FONT_POINT_SIZE), 1.0f);
            float x = (WINDOW_WIDTH_PX - lineSize.x) / 2.0f;
            float y = startY + static_cast<float>(i) * HUD_LINE_HEIGHT_PX;
            DrawTextEx(GetFontDefault(), lines[i].c_str(), Vector2{x, y},
                       static_cast<float>(FONT_POINT_SIZE), 1.0f, WHITE);
        }
        EndDrawing();

        if (key != 0) return key;
    }
    return 0;
}

void showKeyBindingsScreen(RaylibContext& context) {
    std::vector<std::string> lines = {
        "KEYS: PLAYER 1",
        "-------------------------",
        "E - RIGHT track forward",
        "D - RIGHT track backward",
        "Q - LEFT track forward",
        "A - LEFT track backward",
        "S - STAY",
        "W - Shoot",
        "Z - Switch active tank",
        "",
        "KEYS: PLAYER 2",
        "-------------------------",
        "O - RIGHT track forward",
        "L - RIGHT track backward",
        "U - LEFT track forward",
        "J - LEFT track backward",
        "K - STAY",
        "I - Shoot",
        "M - Switch active tank",
        "",
        "Press any key to return to the main menu...",
    };
    waitForKeyPress(context, lines);
}

} // namespace

MenuChoice MenuScreens::showMainMenu(RaylibContext& context) {
    while (!context.shouldQuit()) {
        std::vector<std::string> lines = {
            "(1) Start a new game",
            "(8) Show controls",
            "(9) EXIT",
        };
        int key = waitForKeyPress(context, lines);
        if (key == 0) return MenuChoice::EXIT;

        if (key == MENU_START_GAME_KEY) return MenuChoice::START_GAME;
        if (key == MENU_EXIT_KEY) return MenuChoice::EXIT;
        if (key == MENU_SHOW_INSTRUCTIONS_KEY) {
            showKeyBindingsScreen(context);
        }
    }
    return MenuChoice::EXIT;
}

OpponentType MenuScreens::promptOpponentType(RaylibContext& context) {
    std::vector<std::string> lines = {
        "Before the game starts, choose an opponent:",
        "(1) Human vs Human",
        "(2) Human vs Computer",
    };
    while (!context.shouldQuit()) {
        int key = waitForKeyPress(context, lines);
        if (key == OPPONENT_HUMAN_KEY) return OpponentType::HUMAN;
        if (key == OPPONENT_COMPUTER_KEY) return OpponentType::COMPUTER;
    }
    return OpponentType::HUMAN;
}

Difficulty MenuScreens::promptDifficulty(RaylibContext& context) {
    std::vector<std::string> lines = {
        "Choose a difficulty:",
        "(1) Easy",
        "(2) Hard",
    };
    while (!context.shouldQuit()) {
        int key = waitForKeyPress(context, lines);
        if (key == DIFFICULTY_EASY_KEY) return Difficulty::EASY;
        if (key == DIFFICULTY_HARD_KEY) return Difficulty::HARD;
    }
    return Difficulty::EASY;
}
