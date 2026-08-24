#include "handlers/game_controller.h"
#include "raylib-cpp.hpp"

#include <algorithm>
#include <string>

#include "logic/human_player.h"
#include "logic/computer_player.h"
#include "logic/shell_movement.h"
#include "logic/wall_logic.h"
#include "logic/collision_rules.h"
#include "handlers/scene_renderer.h"
#include "handlers/hud_renderer.h"
#include "handlers/menu_screens.h"

void GameController::run() {
    while (true) {
        MenuChoice choice = MenuScreens::showMainMenu(context);
        if (choice == MenuChoice::EXIT || context.shouldQuit()) return;

        OpponentType opponentType = MenuScreens::promptOpponentType(context);
        if (context.shouldQuit()) return;

        setupGame(opponentType == OpponentType::COMPUTER);
        runGameLoop();
        // runGameLoop() returns on window-close, on X-during-pause (which the
        // pause message itself promises returns to the main menu), or on a
        // win/tie. Loop back to the main menu in every case except a real
        // window close, which the top of the loop catches on its next pass.
    }
}

void GameController::setupGame(bool player2IsComputer) {
    board = Board();

    std::vector<Point> tankPositionsPlayer1 = {
        Point(TANK1_INIT_POS_X, TANK1_INIT_POS_Y),
        Point(TANK2_INIT_POS_X, TANK2_INIT_POS_Y)
    };
    std::vector<Point> tankPositionsPlayer2 = {
        Point(TANK1_INIT_POS_X + PLAYER2_SPAWN_OFFSET_ROW, TANK1_INIT_POS_Y + PLAYER2_SPAWN_OFFSET_COL),
        Point(TANK2_INIT_POS_X + PLAYER2_SPAWN_OFFSET_ROW, TANK2_INIT_POS_Y + PLAYER2_SPAWN_OFFSET_COL)
    };

    player1 = std::make_unique<HumanPlayer>(tankPositionsPlayer1, TANK1_FLAG, TANK_SYMBOL_PLAYER_1, board,
                                             input, PLAYER1_KEYS);
    if (player2IsComputer) {
        player2 = std::make_unique<ComputerPlayer>(tankPositionsPlayer2, TANK1_FLAG, TANK_SYMBOL_PLAYER_2, board);
    } else {
        player2 = std::make_unique<HumanPlayer>(tankPositionsPlayer2, TANK1_FLAG, TANK_SYMBOL_PLAYER_2, board,
                                                 input, PLAYER2_KEYS);
    }

    shells.clear();

    for (int i = 0; i < NUM_OF_MINES; ++i) {
        mines[i] = Mine(Point(MINE_POS_X[i], MINE_POS_Y[i]));
    }

    walls.clear();
    for (const WallCoord& w : WALL_LAYOUT) {
        walls.push_back(Wall(Point(w.row, w.col)));
    }
}

void GameController::renderFrame() {
    BeginDrawing();
    ClearBackground(BLACK);
    SceneRenderer::render(board, mines);
    HudRenderer::renderLives(*player1, *player2);
    EndDrawing();
}

void GameController::showEndScreen(const char* message) {
    std::string fullMessage = std::string(message) + "   (press any key to continue)";

    // Discard any leftover key-press events queued up during gameplay
    // (movement/fire input never drains this queue), so only a genuinely
    // fresh press counts here.
    while (GetKeyPressed() != 0) {}

    while (!context.shouldQuit()) {
        int key = GetKeyPressed();

        BeginDrawing();
        ClearBackground(BLACK);
        SceneRenderer::render(board, mines);
        HudRenderer::renderLives(*player1, *player2);
        HudRenderer::renderEndMessage(fullMessage.c_str());
        EndDrawing();

        if (key != 0) return;
    }
}

bool GameController::handlePause() {
    while (!context.shouldQuit()) {
        BeginDrawing();
        ClearBackground(BLACK);
        SceneRenderer::render(board, mines);
        HudRenderer::renderLives(*player1, *player2);
        HudRenderer::renderPauseOverlay();
        EndDrawing();

        if (IsKeyPressed(ESCAPE_KEY_CODE)) return false;
        if (IsKeyPressed(QUIT_KEY_CODE)) {
            showEndScreen("Goodbye comrades!!! and always remember: the one in the tank wins!!!");
            return true;
        }
    }
    return true;
}

bool GameController::runOneTick() {
    player1->decideActions(board, *player2, shells);
    player2->decideActions(board, *player1, shells);
    input.clearLatch();
    player1->applyMovement(board);
    player2->applyMovement(board);

    for (auto& shell : shells) {
        ShellMovementLogic::advance(shell, board);
    }
    shells.erase(
        std::remove_if(shells.begin(), shells.end(), [](const Shell& s) { return !s.isActive(); }),
        shells.end());

    CollisionRules::resolveShellCollisions(shells, *player1, *player2, walls, board);
    CollisionRules::resolveMineCollisions(mines, *player1, *player2, shells, board);

    walls.erase(
        std::remove_if(walls.begin(), walls.end(), [](const Wall& w) { return w.isDestroyed(); }),
        walls.end());
    for (auto& wall : walls) {
        WallLogic::syncToBoard(wall, board);
    }

    if (player1->allTanksDead() && player2->allTanksDead()) {
        showEndScreen("It's a tie!");
        return true;
    }
    if (player1->allTanksDead()) {
        showEndScreen("Congratulations!!! player 2 won the battle.");
        return true;
    }
    if (player2->allTanksDead()) {
        showEndScreen("Congratulations!!! player 1 won the battle.");
        return true;
    }

    player1->cooldownTickAllTanks();
    player2->cooldownTickAllTanks();
    return false;
}

void GameController::runGameLoop() {
    double accumulatedTime = 0.0;

    while (true) {
        if (context.shouldQuit()) return;

        if (IsKeyPressed(ESCAPE_KEY_CODE)) {
            if (handlePause()) return;
            if (context.shouldQuit()) return;
            accumulatedTime = 0.0;
            input.clearLatch();
        }

        input.pollFrame();
        accumulatedTime += GetFrameTime();
        while (accumulatedTime >= GAME_TICK_SECONDS) {
            accumulatedTime -= GAME_TICK_SECONDS;
            if (runOneTick()) return;
        }

        renderFrame();
    }
}
