#include "fight_stage.h"

FightStage::FightStage() : Stage("FightStage") {
	//empty constructor
};

void FightStage::render() {
	drawText(2, 20, "FightStage", Vector3(1, 1, 1), 5);
};

void FightStage::update() {};
