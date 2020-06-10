#include "stage.h"
#include "input.h"


Stage::Stage(const char* name) {
	stages[name] = this;
	if (current_stage == NULL) current_stage = this;
}

void Stage::render() {};
void Stage::update() {};