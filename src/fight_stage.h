#ifndef FIGHT_STAGE_H
#define FIGHT_STAGE_H

#include "utils.h"
#include "stage.h"

class FightStage : public Stage
{
public:
	FightStage();
	void render();
	void update();
};

#endif