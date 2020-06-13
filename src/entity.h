#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include "texture.h"
#include "utils.h"
#include "animation.h"
#include <string>


const std::vector<std::string> item_names = {"Pocion de Vida", "Pocion de Stamina"};
enum  Item { pocionHP, pocionST };
const std::vector<std::string> enemy_names = { "Esqueleto", "Orco" , "Golem de Piedra" , "Rery de los Monstruos"};
enum  EnemyType { esqueleto , orco , golem , boss};


class Entity {
public:

	//basic characteristics
	Matrix44 model;
	Mesh* mesh = NULL;
	Texture* texture = NULL;
	Vector3 pos;
	float angle = 0.f;

	//Animations
	Skeleton* skeleton;
	Animation* dance_anim;
	Animation* run_anim;
	Animation* idle_anim;
	Animation* attack_anim;

	//playable characteristics
	float life = 100.f;
	float max_life = 100.f;
	float light_atk = 30.f;
	float heavy_atk = 55.f;

	Entity();
	Entity(Vector3 p, int r , float s);
	Entity(const char* m, const char* t);
	Entity(const char* m, const char* t, Vector3 p, int r, float s);
	void movePos(Vector3 p);
	void changeView(float v);
	void setModelPos(Vector3 p);
};



#endif
