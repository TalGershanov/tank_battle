#pragma once
#include <vector>
#include "data/board.h"
#include "data/shell.h"
#include "data/tank.h"

enum TankFlag { TANK1_FLAG, TANK2_FLAG };

// Abstract base: HumanPlayer and ComputerPlayer both implement decideActions,
// so GameController can drive either uniformly through a Player&.
class Player {
protected:
    std::vector<Tank> myTanks;
    int tankIndex;
    int score;
    char tankSymbol;

public:
    Player(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board);
    virtual ~Player() = default;

    // Decide (but don't yet apply) this tick's actions: track statuses and
    // any new shells fired. Implemented differently by HumanPlayer (reads
    // input) and ComputerPlayer (AI heuristic).
    virtual void decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) = 0;

    void applyMovement(Board& board);
    void changeTankFlag();
    void addToScore(int addition);
    int getScore() const;
    Tank& getActiveTank();
    std::vector<Tank>& getTankVector();
    bool allTanksDead() const;
    void cooldownTickAllTanks();
};
