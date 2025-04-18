#ifndef GAME_H
#define GAME_H
#include "tank.h"
using std :: cin;

class Game {
public:
    void run();;
    void showMenu();
    void showInstructions();
    void handleInput(char pressedKey,Tank& tank);

};



#endif //GAME_H
