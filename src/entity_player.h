#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "entity.h"

class Player : public Entity {
public:

	float speed = 0.f;
	float max_speed = 5;
	std::vector<Item> inventory[10];

	Player();
	Player(const char* m, const char* t, Vector3 pos);
};


#endif
