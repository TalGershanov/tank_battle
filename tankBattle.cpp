#include "handlers/game_controller.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    GameController gameController;
    gameController.run();
    return 0;
}
