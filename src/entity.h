#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include <string>

class Entity {
public:
	
	Matrix44 model;
	Vector3 pos;
	float speed = 0.005f;

	Entity();
	void movePos(Vector3 p);
	void changeView(float v);
};

#endif
