#pragma once
#include "logic/player.h"

class HudRenderer {
public:
    static void renderLives(Player& player1, Player& player2);
    static void renderPauseOverlay();
    static void renderEndMessage(const char* text);
};
