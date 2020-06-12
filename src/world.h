#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include <fstream>
#include <sstream>
#include "shader.h"
#include "entity.h"
#include "entity_enemy.h"
#include "entity_player.h"

void chooseModel(Matrix44 * m, int tile, int * index);

class World {
public:
	int w;
	int h;
	int * map = NULL;
	Texture * minimap;

	Player player;
	Entity espada;
	Entity tenosuke;
	std::vector<Enemy> enemies;

	bool isCellEmpty(float pos_x, float pos_y);
	bool loadMap(std::string filesrc); 

	void generateMap(int* map, int w, int h);
	void renderMap(int* map, int w, int h, Shader* shad);
	std::vector<Entity> getNearEntities(float pos_x, float pos_y);
	void loadScene(char* scene_name);
};

#endif
