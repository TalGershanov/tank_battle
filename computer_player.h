
#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H
#include "player.h"
#include "shell.h"


class ComputerPlayer : public Player {

public:
    ComputerPlayer(const std::vector<Point> &tankPositions_, int _tankIND, char _tankSymbol, Board &board_, int _printColor);
    void movementDecision(Board &board, Player &opponent, std::vector<Shell> &shells);
    void moveTanks(Board &board,  Player &opponent, std::vector<Shell> &shells) override;
private:
    // Helper functions
    bool isOpponentInLineOfFire(const Tank &tank,  Player &opponent, const Board &board) const;
    bool isFriendlyInLineOfFire( Tank &tank, const Board &board) ;
    bool isShellChasingTank(const Tank &tank, const std::vector<Shell> &shells) const;
    void evadeShell(Tank &tank, Board &board);

};


#endif //COMPUTER_PLAYER_H
