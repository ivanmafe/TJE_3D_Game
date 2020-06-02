#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include <fstream>
#include <sstream>
#include "texture.h"

void chooseModel(Matrix44 * m, int tile, int * index);

class World {
public:
	int w;
	int h;
	int * map = NULL;

	bool isCellEmpty(float pos_x, float pos_y);
	bool loadMap(std::string filesrc); 
	bool loadVegetation(const char* filesrc);

};

#endif
