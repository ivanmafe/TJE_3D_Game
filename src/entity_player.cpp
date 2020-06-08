#include "entity_player.h"

Player::Player():Entity() {
}


Player::Player(const char* m , const char* t, Vector3 p):Entity(m,t) {

	pos = p;
	setModelPos(p);

}
