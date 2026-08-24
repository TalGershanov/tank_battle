#pragma once
#include "data/board.h"
#include "data/tank.h"

// Ports the original Tank constructor's spawn logic: check whether the spawn
// point is fully surrounded, and if not, pick a random open direction.
Tank spawnTankAt(Point p, const Board& board);
