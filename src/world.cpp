#include "world.h"
#include "utils.h"


bool World::isCellEmpty(int x, int y) {
	int val = map[x * w + y];
	int t[] = { 0, 2, 20 , 22 , 13, 14 , 3, 4, 10, 21, 21 , 1 , 12 , 5, 6, 7 };
	for (int i = 0; i < 16; i++)
		if (x == t[i]) return false;
	return true;
}

bool World::loadMap(std::string filesrc) { //archivo y tamaño de area
	
	std::fstream file;
	file.open(filesrc, std::fstream::in);
	if (!file.is_open()) {
		fprintf(stderr, "Error locating the file map");
		return 0;
	}

	//get map header containing dimensions
	file >> w;
	file >> h;
	int size = w * h;

	//reallocate memory of the map
	if (map != NULL)
		free(map);
	map = (int*)calloc(w * h, sizeof(int));

	int pos = 0;
	int aux = 0;
	while (file.good()) {
		std::string line;
		while (getline(file, line)) {   // get a whole line
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				int aux;
				std::istringstream(line) >> aux;
				map[pos++] = aux;
			}
		}
	}
	return true;
}