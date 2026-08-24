#ifndef GAME_H
#define GAME_H
#include "tank.h"
#include "shell.h"
#include "board.h"
#include "player.h"
#include <conio.h>
#include <Windows.h>
#include <vector>
#include "wall.h"
#include "mine.h"
#include <functional>
#include <algorithm>
#include "general.h"


using std :: cin;

class Game {
private:
    bool printKeys();
public:
    void run();
   // void showMenu();
    int showInstructions();
    void handleInput(Player &player1, Player &player2, std::vector<Shell>& shells );

};



#endif //GAME_H
