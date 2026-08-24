#pragma once
#include "data/board.h"
#include "data/wall.h"

class WallLogic {
public:
    static char symbolFor(const Wall& wall);
    static void syncToBoard(const Wall& wall, Board& board);
};
