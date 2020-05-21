#include "entity.h"
#include "utils.h"

Entity::Entity() {
}

void Entity::movePos(Vector3 p) {

	model.translate(p.x, p.y, p.z);
	pos = model.getTranslation();
}

void Entity::changeView(float v) {
	
	Vector3 t =	model.getTranslation();

	model.m[12] = 0;
	model.m[14] = 0;

	model.rotate(v * DEG2RAD, Vector3(0,1,0));
	
	model.m[12] = t.x;
	model.m[14] = t.z;
}

void Entity::setModelPos(Vector3 p) {
	model.m[12] = p.x;
	model.m[13] = p.y;
	model.m[14] = p.z;
}