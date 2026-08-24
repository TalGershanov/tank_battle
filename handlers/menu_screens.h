#pragma once
#include "handlers/raylib_context.h"

enum class MenuChoice { START_GAME, EXIT };
enum class OpponentType { HUMAN, COMPUTER };

class MenuScreens {
public:
    // Loops on the 3-option main menu (Start / Instructions & keys / Exit)
    // until the player chooses to start or exit. Also returns EXIT if the
    // window is closed.
    static MenuChoice showMainMenu(RaylibContext& context);

    static OpponentType promptOpponentType(RaylibContext& context);
};
