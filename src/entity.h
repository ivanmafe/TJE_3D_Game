#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include "texture.h"
#include "utils.h"
#include <string>


const std::vector<std::string> item_names = {"Pocion de Vida", "Pocion de Stamina"};
enum  Item { pocionHP, pocionST };
const std::vector<std::string> enemy_names = { "Esqueleto", "Ogro" , "Golem de Piedra" };
enum  EnemyType { esqueleto , ogro , golem };


class Entity {
public:

	//basic characteristics
	Matrix44 model;
	Mesh* mesh = NULL;
	Texture* texture = NULL;
	Vector3 pos;
	float angle = 0.f;
	int momentum = 0;
	bool wasHit = false;
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
