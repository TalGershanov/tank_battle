#ifndef WALL_H
#define WALL_H

#include <stdbool.h>
#include "point.h"
#include "board.h"
class Wall {
private:
	Point position;
	int hitsNumber;

public:
	Wall(Point position);
	Point getPosition();
	int getHitsNumber();
	void increaseHits();
	void draw(Board& board);
	bool isDestroyed() const;
};


#endif //WALL_H
