#pragma once
#include "data/board.h"
#include "data/tank.h"

class TankMovementLogic {
public:
    // Mechanical port of the original Tank::move() state machine, minus drawing.
    static void advance(Tank& tank, Board& board, char tankSymbol);

    static char cannonSymbolFor(Direction dir);

    // Blanks the tank's body+cannon cells in the board grid (was Tank::removeDeadTank).
    static void removeDeadTankFromBoard(const Tank& tank, Board& board);

    // Clears the tank's cannon and blanks its (now-collapsed-to-position) cell
    // in the board grid (was Tank::removeCannon) — preserves the original's
    // quirk of blanking the tank's position cell, not the true prior cannon cell.
    static void clearTankCannonFromBoard(Tank& tank, Board& board);
};
