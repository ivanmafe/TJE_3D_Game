#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include <fstream>
#include <sstream>

void chooseModel(Matrix44 * m, int tile, int * index);

class World {
public:
	int w;
	int h;
	int * map = NULL;

	bool isCellEmpty(int x, int y);
	bool loadMap(std::string filesrc);

};

#endif
