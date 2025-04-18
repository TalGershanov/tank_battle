#include "game.h"
#include "board.h"
#include "tank.h"
#include <conio.h>

void Game :: showMenu() {
    cout<< "show instructions -> 0" << "start -> 1" <<endl<<"pause -> 2"<<endl<<"exit -> 3"<<endl;

};

void Game :: handleInput(char pressedKey,Tank& tank) {
    switch (pressedKey) {
        case 'q':
        case 'e':
            tank.setDirection(Direction::UP);
            break;
        case 'a':
        case 'd':
            tank.setDirection(Direction::DOWN);
            break;
        case 's':
            tank.setDirection(Direction::NONE);
            break;
    }
}

void Game :: run() {
    Board board;
    Tank tank(Point(20, 20));

    while (true) {
        if (_kbhit()) {
            int keyPressed = _getch();
            if (keyPressed == 27) { //ESC
                break;
            }
            handleInput((char)keyPressed, tank);
        }

        board.clear();
        tank.move();
        tank.draw(board);
        board.display();
    }
}
