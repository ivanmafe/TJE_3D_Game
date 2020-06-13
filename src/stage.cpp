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
char* Stage::returnMission() { return("Stage"); };
void Stage::setMission(int m) {};