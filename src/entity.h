#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include "texture.h"
#include "utils.h"
#include <string>

class Entity {
public:

	Matrix44 model;
	Vector3 pos;
	float speed = 0.015f;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float angle;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mesh * mesh = NULL;
	Texture * texture = NULL;
	/* For future battle interaction (enemies and hero have these)
	float life = 100;
	float att_speed = 0.5;
	float light_dmg = 25;
	float heavy_dmg = 40;
	*/

	Entity();
	void movePos(Vector3 p);
	void changeView(float v);
	void setModelPos(Vector3 p);
};

#endif
