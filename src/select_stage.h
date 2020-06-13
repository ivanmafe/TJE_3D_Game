#ifndef SELECT_STAGE_H
#define SELECT_STAGE_H

#include "utils.h"
#include "stage.h"

class SelectStage : public Stage
{
public:
	int actual = 1;
	SelectStage();
	void render();
	void update(double seconds_elapsed);
	char* returnMission();
};

#endif