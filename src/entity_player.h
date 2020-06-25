#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "entity.h"
#include "utils.h"

class Player : public Entity {
public:

	float moveAngle = 0.f;
	float speed = 0.f;
	float max_speed = 5;
	int momentum = 0;
	std::vector<Item> inventory[10];


	Player();
	Player(const char* m, const char* t, Vector3 pos);
};


#endif
