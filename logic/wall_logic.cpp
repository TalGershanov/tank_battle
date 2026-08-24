#include "logic/wall_logic.h"

char WallLogic::symbolFor(const Wall& wall) {
    if (wall.getHitsNumber() == WALL_HITS_STRONG) return STRONG_WALL_SYMBOL;
    if (wall.getHitsNumber() == WALL_HITS_WEAK) return WEAK_WALL_SYMBOL;
    return EMPTY_CELL_SYMBOL;
}

void WallLogic::syncToBoard(const Wall& wall, Board& board) {
    board.setBoardCell(wall.getPosition(), symbolFor(wall));
}
