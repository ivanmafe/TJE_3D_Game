#ifndef ENTITY_ENEMY_H
#define ENTITY_ENEMY_H

#include "entity.h"
#include "utils.h"

class Enemy : public Entity {
public:
	Item loot;
	EnemyType type;
	float range = 7.5f;
	bool wasHit = false;
	Enemy();
	Enemy(const char* m, const char* t, Vector3 p, int r, float s, int enemy_type);
};


#endif