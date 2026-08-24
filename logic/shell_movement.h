#pragma once
#include "data/board.h"
#include "data/shell.h"

class ShellMovementLogic {
public:
    static void advance(Shell& shell, Board& board);
};
