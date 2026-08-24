#include "logic/tank_spawn.h"
#include <cstdlib>

Tank spawnTankAt(Point p, const Board& board) {
    bool isSurrounded = true;
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (board.isEmptyPos(p.NextPointIs(static_cast<Direction>(i))))
            isSurrounded = false;
    }

    Direction direction;
    bool hasCannon;

    if (!isSurrounded) {
        direction = static_cast<Direction>(rand() % NUM_DIRECTIONS);
        while (!(board.isEmptyPos(p.NextPointIs(direction)))) {
            direction = static_cast<Direction>(rand() % NUM_DIRECTIONS);
        }
        hasCannon = true;
    } else {
        hasCannon = false;
        direction = Direction::UP;
    }

    return Tank(p, direction, hasCannon);
}
