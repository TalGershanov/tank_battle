#include "general.h"


void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clrscr(){
    system("cls");
}

bool isKeyPressed(int key) {
    return GetAsyncKeyState(key) & 0x8000;
}

bool isLetter(const char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
};
