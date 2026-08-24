#ifndef GENERAL_H
#define GENERAL_H
#include <iostream>
#include <conio.h>
#include <Windows.h>


using std :: cout;
using std :: endl;
constexpr int BEGIN_BOARD_X = 30;
constexpr int BEGIN_BOARD_Y = 0;
constexpr int BOARD_WIDTH = 40;
constexpr int BOARD_HEIGHT = 20;
constexpr int MIN_ROW = 1;
constexpr int MAX_ROW = BOARD_HEIGHT-2;
constexpr int MIN_COL = 1;
constexpr int MAX_COL = BOARD_WIDTH-2;
constexpr int NUM_OF_MINES = 6;


constexpr char BOARDER_SYMBOL = '+';
constexpr char TANK_SYMBOL_PLAYER_1 = '1';
constexpr char TANK_SYMBOL_PLAYER_2 = '2';
constexpr char SHELL_SYMBOL = '*';

constexpr char STRONG_WALL_SYMBOL = '#';
constexpr char WEAK_WALL_SYMBOL = 'H';
constexpr int MAX_WALL_HITS_NUM = 2;
constexpr char MINE_SYMBOL = '@';


constexpr int MAX_COOLDOWN = 5;
constexpr int MIN_COOLDOWN = 0;

constexpr char CANNON_UP_OR_DOWN_SYMBOL = '|';
constexpr char CANNON_UL_OR_DR_SYMBOL = '\\';
constexpr char CANNON_DL_OR_UR_SYMBOL = '/';
constexpr char CANNON_L_OR_R_SYMBOL = '-';

constexpr int TANK1_INIT_POS_X = BOARD_HEIGHT/4;
constexpr int TANK1_INIT_POS_Y = BOARD_WIDTH/4;
constexpr int TANK2_INIT_POS_X = (BOARD_HEIGHT*3)/4;
constexpr int TANK2_INIT_POS_Y = (BOARD_WIDTH*3)/4;

enum class Direction {UP, UP_LEFT , LEFT , DOWN_LEFT , DOWN , DOWN_RIGHT , RIGHT , UP_RIGHT, NONE};

constexpr int MINE_POS_X[] = {
        BOARD_HEIGHT / 6,    // MINE_1_X
        (BOARD_HEIGHT * 2) / 6,  // MINE_2_X
        (BOARD_HEIGHT * 3) / 6,  // MINE_3_X
        (BOARD_HEIGHT * 4) / 6,  // MINE_4_X
        (BOARD_HEIGHT * 5) / 6,  // MINE_5_X
        MAX_ROW              // MINE_6_X
};

constexpr int MINE_POS_Y[] = {
        BOARD_WIDTH / 6,    // MINE_1_Y
        (BOARD_WIDTH * 2) / 6,  // MINE_2_Y
        (BOARD_WIDTH * 3) / 6,  // MINE_3_Y
        (BOARD_WIDTH * 4) / 6,  // MINE_4_Y
        (BOARD_WIDTH * 5) / 6,  // MINE_5_Y
        MAX_COL              // MINE_6_Y
};

void gotoxy(int x, int y);
void clrscr();
bool isKeyPressed(int key);
bool isLetter(const char c);
#endif //GENERAL_H
