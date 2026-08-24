#pragma once
#include "constants/constants.h"
#include "logic/input_provider.h"
#include "logic/player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board,
                const IInputProvider& input, KeyBindings keys);

    void decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) override;

private:
    const IInputProvider& input;
    KeyBindings keys;
};
