#ifndef DEBUG_STAGE_H
#define DEBUG_STAGE_H

#include "utils.h"
#include "stage.h"

class DebugStage : public Stage
{
public:
	float camera_speed = 0.05;
	DebugStage();
	void render();
	void update();
};

#endif