#include "stage.h"
#include "input.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "game.h"


Stage::Stage(const char* name) {
	stages[name] = this;
	if (current_stage == NULL) current_stage = this;
}

void Stage::render() {};
void Stage::update(double seconds_elapsed) {};
int Stage::returnActualVal() { return -1; };
int Stage::returnNextVal() { return -1; };
char* Stage::returnMission() { return("Stage"); };
void Stage::setActual(int m) {};
void Stage::setNext(int m) {};
void Stage::missionUP() {
	max_mission++;
};
int Stage::getMaxMission(){
	return max_mission;
}