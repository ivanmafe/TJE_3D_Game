#ifndef PLAY_STAGE_H
#define PLAY_STAGE_H

#include "utils.h"
#include "stage.h"

class PlayStage : public Stage
{
public:
	PlayStage();
	void render();
	void update(double seconds_elapsed);
};

#endif