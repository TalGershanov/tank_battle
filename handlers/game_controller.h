#pragma once
#include <array>
#include <memory>
#include <vector>
#include "constants/constants.h"
#include "data/board.h"
#include "data/mine.h"
#include "data/wall.h"
#include "data/shell.h"
#include "logic/player.h"
#include "handlers/raylib_context.h"
#include "handlers/raylib_input_provider.h"

class GameController {
public:
    void run();

private:
    void setupGame(bool player2IsComputer);
    void runGameLoop();
    // Advances the simulation by exactly one game tick. Returns true if the
    // game ended (win/tie) this tick.
    bool runOneTick();
    // Returns true if the player chose to quit to the main menu (X pressed
    // while paused); false if they resumed (ESC pressed again).
    bool handlePause();
    void renderFrame();
    // Renders `message` and blocks (rendering every frame) until a key is
    // pressed or the window is closed.
    void showEndScreen(const char* message);

    RaylibContext context;
    RaylibInputProvider input;
    Board board;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::vector<Shell> shells;
    std::vector<Wall> walls;
    std::array<Mine, NUM_OF_MINES> mines;
};
