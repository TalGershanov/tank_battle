
#include "mine.h"

Mine::Mine(Point p){
    visibility= true;
    destroyed = false;
    position = p;

}
Mine :: Mine(): destroyed(false) , visibility(true){};

Point Mine ::getPosition(){
    return position;
};

bool Mine ::isDestroyed() {
    return destroyed;
};

void Mine ::setDestroyed() {
    destroyed = true;
};

void Mine ::draw(){
    if (visibility && !destroyed) {
        gotoxy(position.getY() + BEGIN_BOARD_X, position.getX() + BEGIN_BOARD_Y);
        cout << MINE_SYMBOL;
    }

};

void Mine ::markAsVisible(){
    visibility = true;
};

void Mine ::markAsInvisible(){
    visibility = false;
};
