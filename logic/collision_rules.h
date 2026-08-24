#pragma once
#include <array>
#include <vector>
#include "constants/constants.h"
#include "data/board.h"
#include "data/mine.h"
#include "data/shell.h"
#include "data/wall.h"
#include "logic/player.h"

class CollisionRules {
public:
    // Each shell checks targets in priority order (player1 tanks -> player2
    // tanks -> other shells -> walls) and stops at the FIRST match: it
    // explodes on the first thing it hits instead of registering multiple
    // effects in the same tick.
    static void resolveShellCollisions(std::vector<Shell>& shells, Player& player1, Player& player2,
                                        std::vector<Wall>& walls, Board& board);

    static void resolveMineCollisions(std::array<Mine, NUM_OF_MINES>& mines, Player& player1, Player& player2,
                                       const std::vector<Shell>& shells, Board& board);
};
