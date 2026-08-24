#include "game.h"


int main() {
    // this makes sure rand() doesnt give the same sequance each time
    srand(static_cast<unsigned int>(time(nullptr)));
    Game game;
    game.run();
    gotoxy(0, 100);
}
