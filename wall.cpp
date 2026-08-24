
#include "wall.h"
#include "Board.h"


Wall::Wall(Point position) {
	this->position = position;
	this->hitsNumber = 0;
}

Point Wall::getPosition() {
	return this->position;
}

int Wall::getHitsNumber() {
	return this->hitsNumber;
}

void Wall::increaseHits()
{
	if (hitsNumber < MAX_WALL_HITS_NUM)
		this->hitsNumber++;
}

bool Wall::isDestroyed() const {
	return hitsNumber >= MAX_WALL_HITS_NUM;
}


void Wall::draw(Board& board) {
	char wall_symbol;

	if (hitsNumber == 0)
		wall_symbol = STRONG_WALL_SYMBOL;
	else if (hitsNumber == 1)
		wall_symbol = WEAK_WALL_SYMBOL;
	else
		wall_symbol = ' ';

	board.setBoardCell(position, wall_symbol);
	gotoxy(position.getY() + BEGIN_BOARD_X, position.getX() + BEGIN_BOARD_Y);
	std::cout << wall_symbol;
}
