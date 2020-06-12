#include "entity_enemy.h"


Enemy::Enemy() {}

Enemy::Enemy(const char* m, const char* t, Vector3 p, int r, float s, int enemy_type) : Entity(m,t,p,r,s){

	loot = static_cast<Item>(rand() % item_names.size());
	type = static_cast <EnemyType>(enemy_type);

	pos = p;
	setModelPos(p);
	if (r != 0) model.rotate(r * DEG2RAD, Vector3(0, 1, 0));
	if (s != 0.f) model.scale(s, s, s);
	mesh = Mesh::Get(m);
	texture = Texture::Get(t);
}