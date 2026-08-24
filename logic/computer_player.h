#pragma once
#include "logic/player.h"

class ComputerPlayer : public Player {
public:
    ComputerPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board);

    void decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) override;

private:
    bool isOpponentInLineOfFire(const Tank& tank, Player& opponent, const Board& board) const;
    bool isFriendlyInLineOfFire(Tank& tank, const Board& board);
    bool isShellChasingTank(const Tank& tank, const std::vector<Shell>& shells) const;
    void evadeShell(Tank& tank);
};
