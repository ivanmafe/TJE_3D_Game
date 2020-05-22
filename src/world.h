#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <sstream>

class World {
public:
	int w;
	int h;
	int * map = NULL;

	bool loadMap(std::string filesrc);

};

#endif
