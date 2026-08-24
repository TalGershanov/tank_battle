
#ifndef PLAYER_H
#define PLAYER_H
#include "tank.h"
#include "board.h"
#include <conio.h>
#include <Windows.h>
#include <vector>
using std::vector;
enum {TANK1_FLAG , TANK2_FLAG};

class Player {
std::vector<Tank>  myTanks;
int tankIndex;
int score;
char tankSymbol;

public:
    Player(const std::vector<Point>& tankPositions, int tankIND ,  char _tankSymbol, Board& board);
    void changeTankFlag();
    void moveTanks(Board& board);
    void addToScore(int addition);
    int getScore();
    Tank& getActiveTank();
    std::vector<Tank>& getTankVector ();
    bool allTanksDead();
    void cooldownTickAllTanks ();

};


#endif  //PLAYER_H
