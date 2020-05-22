#include "stage.h"

Stage::Stage(const char* name) {
	stages[name] = this;
	if (current_stage == NULL) current_stage = this;
}

void Stage::render() {};
void Stage::update() {};

PlayStage::PlayStage() : Stage("PlayStage") {
	//empty constructor
};

void PlayStage::render() {
	drawText(2,20,"PlayStage",Vector3(1,1,1),5);
};
void PlayStage::update() {};
