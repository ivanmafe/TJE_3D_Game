#include "entity.h"
#include "utils.h"

Entity::Entity() {
}

void Entity::movePos(Vector3 p) {
	pos = pos + p;
	model.translate(p.x, p.y, p.z);
}

void Entity::changeView(float v) {
	
	Vector3 t =	model.getTranslation();

	model.m[12] = 0;
	model.m[14] = 0;
	
	model.rotate(v * DEG2RAD, Vector3(0,1,0));
	
	model.m[12] = t.x;
	model.m[14] = t.z;
}