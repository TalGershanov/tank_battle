#pragma once
#include <array>
#include "constants/constants.h"
#include "data/board.h"
#include "data/mine.h"

// Draws the current game state each tick by reading directly from Board's
// grid (the single source of truth) plus Mine state (mines never touch the
// grid, matching the original). No before/after diffing is needed since the
// whole scene is redrawn every frame.
class SceneRenderer {
public:
    static void render(const Board& board, const std::array<Mine, NUM_OF_MINES>& mines);
};
